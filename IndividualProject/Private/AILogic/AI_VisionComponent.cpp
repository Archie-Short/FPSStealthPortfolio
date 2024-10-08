// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/AI_VisionComponent.h"
#include "DrawDebugHelpers.h" 
#include "AILogic/AICharacter.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values for this component's properties
UAI_VisionComponent::UAI_VisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//Focused Cone Variables
	 FocVisLength = 450;
	 FocVisHeight = 30;
	 FocVisWidth = 30;
	
	//Peripheral Cone Variables
	 PeriphVisLength = 660;
	 PeriphVisHeight = 35;
	 PeriphVisWidth = 35;


	FocusDetectionRate = 20;
	PeriphDetectionRate = 10; 
	 Awareness = 0;
	
	// ...
}

void UAI_VisionComponent::LinkToAI(AAICharacter* AI)
{
	AIMesh	 = AI->GetMesh();
	AIActor = AI;
	TraceParams.AddIgnoredActor(AI);

}


// Called when the game starts
void UAI_VisionComponent::BeginPlay()
{
	Super::BeginPlay();

	FocVisHeight = FMath::DegreesToRadians(FocVisHeight);
	FocVisWidth = FMath::DegreesToRadians(FocVisWidth);
	
	//Peripheral Cone Variables
	PeriphVisHeight = FMath::DegreesToRadians(PeriphVisHeight);
	PeriphVisWidth = FMath::DegreesToRadians(PeriphVisWidth);

	// ...

	TraceParams = FCollisionQueryParams(FName(TEXT("Line Trace")),true);
}

void UAI_VisionComponent::DrawDebugCones(float dt)
{
	UWorld* const World = GetWorld();
	if(World != nullptr)
	{
		//Draw Focused Cone
		DrawDebugCone(World,EyeLocation,EyeDirection,FocVisLength,FocVisWidth,FocVisHeight,40,FColor::Orange,false,-1,0,1.f);
		//Draw Peripheral Cone
		DrawDebugCone(World,EyeLocation,EyeDirection,PeriphVisLength,PeriphVisWidth,PeriphVisHeight,40,FColor::Cyan,false,-1,0,1.f);

	
	}
}

float UAI_VisionComponent::VisibilityScore(FTransform* HeadTransform,FVector ActorLocation, bool bIsPlayer)
{
	FHitResult Hit;
	UWorld* World = GetWorld();
	float Distance = FVector::Distance(ActorLocation,EyeLocation);
	float DistanceModifier = 1- (1/(PeriphVisLength) * Distance);
	
	if(IsValid(World))
	{
		
		//LineTrace
		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, ActorLocation, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), EyeLocation, ActorLocation, FColor::Green, false,0.001f, 0, 2);

			if(Hit.Distance < FVector::Distance(EyeLocation,ActorLocation))
			{

				if(bIsPlayer)
				{
					OnPlayerVisibleDelegate.Broadcast(false, ActorLocation);
				}
				return -PeriphDetectionRate; 
			}
		}
		
		//Check Distance
		
		if(Distance > PeriphVisLength)
		{
			if(bIsPlayer)
			{
				OnPlayerVisibleDelegate.Broadcast(false, ActorLocation);
			}
			return -PeriphDetectionRate; 
		}

		//Get the angles to the location 
		FRotator Rot = UKismetMathLibrary::FindRelativeLookAtRotation(AIMesh->GetSocketTransform(FName(TEXT("VisionSocket"))),ActorLocation);
		float Yaw = FMath::DegreesToRadians(abs(Rot.Yaw)); 
		float Pitch = FMath::DegreesToRadians(abs(Rot.Pitch));
	
		//UE_LOG(LogTemp,Warning,TEXT("Angle To Player (Pitch): %f"),Pitch);
		//UE_LOG(LogTemp,Warning,TEXT("Angle To Player (Yaw): %f"),Yaw);
		//UE_LOG(LogTemp,Warning,TEXT("Cone  (Pitch): %f"),FocVisHeight);
		//UE_LOG(LogTemp,Warning,TEXT("Cone (Yaw): %f"),FocVisWidth);

		
		//Check to see if it is within focused height and width
		if(FVector::Distance(ActorLocation,EyeLocation) <= FocVisLength && Yaw <= FocVisWidth && Pitch <= FocVisHeight)
		{
			if(bIsPlayer)
			{
				OnPlayerVisibleDelegate.Broadcast(true, ActorLocation);
			}
			return FocusDetectionRate*DistanceModifier; 
		}

		//Check to see if it is within peripheral height and width

		if(Yaw <= PeriphVisWidth && Pitch <= PeriphVisHeight){
			if(bIsPlayer)
			{
				OnPlayerVisibleDelegate.Broadcast(true, ActorLocation);
			}
			return PeriphDetectionRate*DistanceModifier; 
		}
	
		//This is if its within distance but not in either angle
		if(bIsPlayer)
		{
			OnPlayerVisibleDelegate.Broadcast(false, ActorLocation);
		}
	}
	
	return -PeriphDetectionRate *  (1-DistanceModifier); 
}

void UAI_VisionComponent::AddToObjectDetectionArray(AActor* Actor)
{
	ActorsToDetect.Add(Actor);
	TraceParams.AddIgnoredActor(Actor); 
}

void UAI_VisionComponent::AddToCharacterDetectionArray(ACharacter* Actor)
{
	CharactersToDetect.Add(Actor);
	TraceParams.AddIgnoredActor(Actor); 
}

void UAI_VisionComponent::UpdateAwareness(float ChangeValue, float DT)
{
	Awareness += ChangeValue * DT;
	if(Awareness > 100.f)
	{
		Awareness = 100.f;
	}
	else if(Awareness < 0){
		Awareness = 0.f; 
	}

	OnAwarenessChangedDelegate.Broadcast(Awareness); 
}

// Called every frame
void UAI_VisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Update Head Transforms
	if(IsValid(AIMesh))
	{
		FTransform HeadTransform = AIMesh->GetSocketTransform(FName(TEXT("VisionSocket")));
		EyeLocation = HeadTransform.GetLocation();
		EyeDirection = HeadTransform.GetRotation().GetNormalized().GetForwardVector();
	
		DrawDebugCones(DeltaTime);

		//Detect the player

	
			for(AActor* Actor:ActorsToDetect)
			{
				//float Score = VisibilityScore(&HeadTransform,Actor->GetActorLocation());
				//UE_LOG(LogTemp,Warning,TEXT("Player Visibility Score: %f"),Score);

				//UpdateAwareness(Score, DeltaTime); 

			}

			TArray<ACharacter*> ToRemove;
			for(ACharacter* Character: CharactersToDetect)
			{
				if(Character->Implements<UEnemy>())
				{
					FVector Location = Character->GetMesh()->GetComponentLocation(); 
					float Score = VisibilityScore(&HeadTransform,Location);
					if(Score > 0)
					{
						UE_LOG(LogTemp,Warning,TEXT("AI has seen the body at position: %s"),*Location.ToString());
						//Trigger action in the behaviour tree
						OnBodySeenDelegate.Broadcast(Location); 
						ToRemove.Add(Character); 
					}
				}
			
			}

		//Detect Player
		float ScoreBody = VisibilityScore(&HeadTransform,Player->GetArrowComponent()->GetComponentLocation(),true);
		float ScoreHead = (VisibilityScore(&HeadTransform,Player->GetFirstPersonCameraComponent()->GetComponentLocation(),true)/2);

		//Fully in sight 
		if(ScoreBody + ScoreHead > 0)
		{
			OnAwarenessChangedDelegate.Broadcast((ScoreBody + ScoreHead) * DeltaTime); 

		}
		else if(ScoreBody > 0)
		{
			OnAwarenessChangedDelegate.Broadcast((ScoreBody) * DeltaTime); 

		}
		else if (ScoreHead > 0 )
		{
			OnAwarenessChangedDelegate.Broadcast((ScoreHead) * DeltaTime); 

		}

		else
		{
			OnAwarenessChangedDelegate.Broadcast((ScoreHead/3) * DeltaTime); 

		}
		


		//Remove characters that need to be removed
		for(ACharacter* remove:ToRemove)
		{
			CharactersToDetect.Remove(remove); 
		}
	
	}
	
}




