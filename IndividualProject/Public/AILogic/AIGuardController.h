// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AILogic/AI_VisionComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "IndividualProject/IndividualProject.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AIGuardController.generated.h"

/**
 * 
 */

struct FActorPerceptionBlueprintInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAgentDeathSignature, AAICharacter*, Guard);

UCLASS()
class INDIVIDUALPROJECT_API AAIGuardController : public AAIController
{
	GENERATED_BODY()

public:
	AAIGuardController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnDeath(); 

	
	FAgentDeathSignature AgentDeathDelegate; 
	
	//Main Components 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Configuration")
	UAI_VisionComponent* VisionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAIPerceptionComponent* HearingComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAISenseConfig_Hearing* HearingConfig; 

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AwarenessLevel1Value;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AwarenessLevel2Value;
	
	UPROPERTY(BlueprintReadOnly)
	float TimeStamp = 0.f;
	
	//Blackboard IDs
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 GuardId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 LocationKeyId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 PlayerVisibleId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 AIStateId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 LastPlayerLocId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 SearchLocId;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 StillSearchingID;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 SearchAttentionID; 
	

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AI")
	AAICharacter* Agent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AI")
	float AgentAwareness; 

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AI")
	uint8 PreviousState; 
	
	//Searching Variables
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Searching")
	bool bSearchStarted;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Searching")
	TArray<FVector> SearchedLocations; 
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Searching")
	int MaxSearchAttention;


	//Player Visible Timer
	FTimerHandle PlayerVisibleTimer;

	bool PlayerVisibleTimerActive;

	int VisibleCallsRemaining; 
 
	void PlayerVisibleTimerFunction(); 

	//Traits
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UPerceptionTraitBase* PerceptionTrait; 

protected:
	UFUNCTION()
	void CheckAwareness(float AwarenessUpdate);

	UFUNCTION()
	void PlayerVisibleUpdate(bool bIsPlayerVisible,FVector LastLocation);

	UFUNCTION()
	void OnHearNoise(TArray<AActor*> const& UpdatedActors);

	UFUNCTION()
	void OnBodySeen(FVector Danger); 
	
	UFUNCTION(BlueprintCallable)
	void AwarenessLevel0();
	
	UFUNCTION(BlueprintCallable)
	void AwarenessLevel1(EAIState CurrState);

	UFUNCTION(BlueprintCallable)
	void AwarenessLevel2();

	UFUNCTION(BlueprintCallable)
	void AwarenessLevel3();

	void CheckIfInvestigateNoise(FAIStimulus &Info);

	
    
    	

	


	
	
	

};


