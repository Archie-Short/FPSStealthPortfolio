// Fill out your copyright notice in the Description page of Project Settings.

#include "AILogic/AIGuardController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AILogic/AICharacter.h"
#include "AILogic/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "IndividualProject/IndividualProject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"

AAIGuardController::AAIGuardController(const FObjectInitializer& ObjectInitializer)
{
	
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Tree Component"));
	VisionComponent = CreateDefaultSubobject<UAI_VisionComponent>(TEXT("Vision Component"));
	HearingComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Hearing Component"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config")); 

	HearingConfig->HearingRange = 4000;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	HearingComponent->ConfigureSense(*HearingConfig); 
	HearingComponent->SetDominantSense(HearingConfig->GetSenseImplementation());
	HearingComponent->OnPerceptionUpdated.AddDynamic(this,&AAIGuardController::OnHearNoise);

	bSearchStarted = false;
	MaxSearchAttention = 4.f; 

	AwarenessLevel1Value = 30.f;
	AwarenessLevel2Value = 70.f;
	PlayerVisibleTimerActive=false;
	VisibleCallsRemaining = 0; 
}

void AAIGuardController::BeginPlay()
{
	Super::BeginPlay();
	if(!Agent)
	{
		AAICharacter* Guard = Cast<AAICharacter>(GetPawn());
		if(!Guard){return;}
		Agent = Guard;
		VisionComponent->LinkToAI(Guard);
		VisionComponent->OnAwarenessChangedDelegate.AddUniqueDynamic(this,&AAIGuardController::CheckAwareness);
		VisionComponent->OnPlayerVisibleDelegate.AddUniqueDynamic(this,&AAIGuardController::PlayerVisibleUpdate);
		VisionComponent->OnBodySeenDelegate.AddUniqueDynamic(this,&AAIGuardController::OnBodySeen);
		
	}

	if(Agent->DefaultBehaviour)
	{
		FGameplayTag SubTag;
		BehaviorTreeComponent->SetDynamicSubtree(SubTag,Agent->DefaultBehaviour->SubTree); 
	}
	

}

void AAIGuardController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AAICharacter* guard = Cast<AAICharacter>(InPawn);
	if(guard != nullptr && guard->TreeAsset != nullptr)
	{
		//Initialise blackboard
		BlackboardComponent->InitializeBlackboard( *guard->TreeAsset->BlackboardAsset);

		//Set blackboard key IDs
		GuardId = BlackboardComponent->GetKeyID("AIGuard");
		LocationKeyId = BlackboardComponent->GetKeyID("MoveToLocation");
		PlayerVisibleId = BlackboardComponent->GetKeyID("PlayerVisible");
		AIStateId = BlackboardComponent->GetKeyID("AIState");
		LastPlayerLocId = BlackboardComponent->GetKeyID("LastPlayerLocation");
		SearchLocId = BlackboardComponent->GetKeyID("SearchLocation");
		StillSearchingID= BlackboardComponent->GetKeyID("SearchStarted"); 
		SearchAttentionID = BlackboardComponent->GetKeyID("SearchAttention"); 
		
		//Start With Patrolling
		BlackboardComponent->SetValueAsEnum("AIState", (uint8)guard->StartState);
		PreviousState = (uint8)guard->StartState; 

		//Set last known location as current just to avoid errors
		BlackboardComponent->SetValueAsVector("LastPlayerLocation",guard->GetActorLocation());

		//Initialise AI into Blackboard
		BlackboardComponent->SetValueAsObject("AIGuard",guard);
		BlackboardComponent->SetValueAsBool("SearchStarted",false);
		BlackboardComponent->SetValueAsInt("SearchAttention",MaxSearchAttention);

		BlackboardComponent->SetValueAsObject("Player",UGameplayStatics::GetPlayerCharacter(GetWorld(),0));


		//start the behaviour tree
		BehaviorTreeComponent->StartTree(*guard->TreeAsset);

		guard->OnDeathDelegate.AddUniqueDynamic(this,&AAIGuardController::OnDeath);



		//Setup Traits
		
		//Perception Trait
		if(guard->AIInformationComponent)
		{
			if(guard->AIInformationComponent->PerceptionTraitClass)
			{
				PerceptionTrait =  NewObject<UPerceptionTraitBase>(GetTransientPackage(),guard->AIInformationComponent->PerceptionTraitClass);
				UE_LOG(LogTemp,Warning,TEXT("Trait Name: %s"),*PerceptionTrait->Title.ToString());
				UE_LOG(LogTemp,Warning,TEXT("Trait Description: %s"),*PerceptionTrait->Description.ToString());
			
				guard->AIInformationComponent->PerceptionTrait = PerceptionTrait; 
				UE_LOG(LogTemp,Warning,TEXT("Attempting Trait Initialisation"));
				PerceptionTrait->InitialisePerceptionTrait(VisionComponent,HearingComponent);
			}

		}



		
	}
}

void AAIGuardController::OnDeath()
{
	BehaviorTreeComponent->StopTree();
	BehaviorTreeComponent->DestroyComponent(); 
	VisionComponent->DestroyComponent();
	AgentAwareness = 0;

	UE_LOG(LogTemp,Warning,TEXT("AI IS NOW DEAD"));
	UE_LOG(LogTemp,Warning,TEXT("Body Position %s"),*Agent->GetMesh()->GetComponentLocation().ToString());

	AgentDeathDelegate.Broadcast(Agent);

	this->Destroy(); 

}

void AAIGuardController::PlayerVisibleTimerFunction()
{
	if(++VisibleCallsRemaining >= 10)
	{
		GetWorldTimerManager().ClearTimer(PlayerVisibleTimer);
		PlayerVisibleTimerActive = false;
		BlackboardComponent->SetValueAsBool("PlayerVisibleDelay",PlayerVisibleTimerActive); 
	}
	else
	{
		PlayerVisibleTimerActive = true;
		BlackboardComponent->SetValueAsBool("PlayerVisibleDelay",PlayerVisibleTimerActive); 
	}
}

void AAIGuardController::CheckAwareness(float AwarenessUpdate)
{
	
	//Increase Awareness
	if(AwarenessUpdate > 0)
	{
		AgentAwareness += AwarenessUpdate;
		if(AgentAwareness >= 200)
		{
			AgentAwareness = 200; 
		}
	}

	EAIState CurrentState =static_cast<EAIState>(BlackboardComponent->GetValueAsEnum("AIState"));

	if(AgentAwareness >= 200)
	{
		AwarenessLevel3();
	}
	else if(AgentAwareness >= 100 && (CurrentState == EAIState::EClarify))
	{
		AwarenessLevel2();
	}
	
	else if(AgentAwareness >= 50 && CurrentState != EAIState::EAlert && CurrentState != EAIState::ESearch)
	{
		AwarenessLevel1(CurrentState); 
	}
	
	//Don't decrease in certain states 
	if(AwarenessUpdate < 0)
	{
		if(CurrentState != EAIState::EEngage&& CurrentState != EAIState::EAlert)
		{
			if(CurrentState == EAIState::ESearch)
			{
				if(BlackboardComponent->GetValueAsInt("SearchAttention") == 0)
				{
					AgentAwareness += AwarenessUpdate;

				}
			}
			else
			{
				AgentAwareness += AwarenessUpdate;
			}
		
			if(AgentAwareness <= 0)
			{
				AgentAwareness = 0; 
			}
		}
	}
	
	Agent->UpdateAwareness(AgentAwareness); 
}

void AAIGuardController::PlayerVisibleUpdate(bool bIsPlayerVisible, FVector LastLocation)
{
	bool LastVisibility = BlackboardComponent->GetValueAsBool("PlayerVisible"); 
	BlackboardComponent->SetValueAsBool("PlayerVisible", bIsPlayerVisible);
	
	if(bIsPlayerVisible)
	{
		BlackboardComponent->SetValueAsVector("LastPlayerLocation",LastLocation);
	}
	else
	{
		if(LastVisibility == true && bIsPlayerVisible == false)
		{
			BlackboardComponent->SetValueAsVector("LastPlayerDirection",UKismetMathLibrary::GetDirectionUnitVector(BlackboardComponent->GetValueAsVector("LastPlayerLocation"),LastLocation)); 

			GetWorldTimerManager().ClearTimer(PlayerVisibleTimer); 
			GetWorldTimerManager().SetTimer(PlayerVisibleTimer,this,&AAIGuardController::PlayerVisibleTimerFunction, 1.f,true);
			PlayerVisibleTimerActive = true;
			BlackboardComponent->SetValueAsBool("PlayerVisibleDelay",PlayerVisibleTimerActive);
			VisibleCallsRemaining = 0;
		}
		else if(VisibleCallsRemaining <= 2)
		{
			BlackboardComponent->SetValueAsVector("LastPlayerDirection",UKismetMathLibrary::GetDirectionUnitVector(BlackboardComponent->GetValueAsVector("LastPlayerLocation"),LastLocation)); 

		}
		//}
		
		
	}
}

void AAIGuardController::OnHearNoise(TArray<AActor*> const& UpdatedActors) 
{
	FAIStimulus* SoundToInvestigate = nullptr; 
	for(int i=0; i<UpdatedActors.Num(); ++i)
	{
		FActorPerceptionBlueprintInfo info;
		HearingComponent->GetActorsPerception(UpdatedActors[i],info);
		FVector stimLocation = info.LastSensedStimuli.Last().StimulusLocation;
		FVector reciever = info.LastSensedStimuli.Last().ReceiverLocation;

		if(SoundToInvestigate == nullptr)
		{
			SoundToInvestigate = &info.LastSensedStimuli.Last(); 
		}
	
		
		UE_LOG(LogTemp,Warning,TEXT("Sound Location: %s"),*stimLocation.ToString());
		UE_LOG(LogTemp,Warning,TEXT("Receiver Location %s"),*reciever.ToString());
		UE_LOG(LogTemp,Warning,TEXT("Distance Between %f"),FVector::Distance(stimLocation,reciever)); 

	}

	if(SoundToInvestigate != nullptr)
	{
		CheckIfInvestigateNoise(*SoundToInvestigate); 

	}
	
}

void AAIGuardController::OnBodySeen(FVector Danger)
{
	EAIState CurrentState = static_cast<EAIState>(BlackboardComponent->GetValueAsEnum("AIState"));

	if(CurrentState == EAIState::EIdle  || CurrentState ==EAIState::EDefault )
	{
		BlackboardComponent->SetValueAsEnum("LastAIState",static_cast<uint8>(CurrentState));
		BlackboardComponent->SetValueAsVector("SearchLocation",Danger);
		FVector Direction = (Danger - Agent->GetActorLocation()).GetUnsafeNormal(); 
		BlackboardComponent->SetValueAsVector("SearchDirection",Direction);
		BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::ESearch));

	}

	
	else if(!(BlackboardComponent->GetValueAsBool("PlayerVisible")) && !(BlackboardComponent->GetValueAsBool("PlayerVisibleDelay"))&& CurrentState!= EAIState::EEngage)
	{
		
		BlackboardComponent->SetValueAsVector("SearchLocation",Danger);
		FVector Direction = (Danger- Agent->GetActorLocation()).GetUnsafeNormal(); 
		BlackboardComponent->SetValueAsVector("SearchDirection",Direction);
		BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::ESearch));

		
	}
}

void AAIGuardController::AwarenessLevel0()
{
	BlackboardComponent->SetValueAsEnum("AIState", PreviousState); 

}

void AAIGuardController::AwarenessLevel1(EAIState CurrState)
{
	if(CurrState != EAIState::EClarify)
	{
		PreviousState = static_cast<int8>(CurrState); 
		BlackboardComponent->SetValueAsEnum("LastAIState", static_cast<int8>(CurrState)); 
		BlackboardComponent->SetValueAsEnum("AIState", static_cast<int8>(EAIState::EClarify));
	}
}

void AAIGuardController::AwarenessLevel2()
{
	BlackboardComponent->SetValueAsEnum("AIState", static_cast<int8>(EAIState::EAlert));
}

void AAIGuardController::AwarenessLevel3()
{
	BlackboardComponent->SetValueAsEnum("AIState", (uint8)EAIState::EEngage);

}

void AAIGuardController::CheckIfInvestigateNoise(FAIStimulus& Info)
{
	EAIState CurrentState = static_cast<EAIState>(BlackboardComponent->GetValueAsEnum("AIState"));

	//Dont investigate noise if the player is visible
	if(CurrentState == EAIState::EIdle  || CurrentState ==EAIState::EDefault )
	{
		BlackboardComponent->SetValueAsEnum("LastAIState",static_cast<uint8>(CurrentState));
		BlackboardComponent->SetValueAsVector("SearchLocation",Info.StimulusLocation);
		BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::EInvestigate));

	}
	if(!(BlackboardComponent->GetValueAsBool("PlayerVisible")) && !(BlackboardComponent->GetValueAsBool("PlayerVisibleDelay"))&& CurrentState!= EAIState::EEngage)
	{
		if(CurrentState != EAIState::EInvestigate)
		{
			BlackboardComponent->SetValueAsEnum("LastAIState",static_cast<uint8>(CurrentState));
			
		}

		if(Agent->Implements<UEnemy>())
		{
			IEnemy::Execute_Bark(Agent,FString(TEXT("Huh, what was that noise?"))); 
		}
		
		
		BlackboardComponent->SetValueAsVector("SearchLocation",Info.StimulusLocation);
		BlackboardComponent->SetValueAsEnum("AIState", static_cast<uint8>(EAIState::EInvestigate));

		
	}
}
