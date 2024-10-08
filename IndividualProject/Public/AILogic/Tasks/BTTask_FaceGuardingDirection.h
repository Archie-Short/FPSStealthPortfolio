// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/AICharacter.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FaceGuardingDirection.generated.h"

/**
 * 
 */
UCLASS()
class INDIVIDUALPROJECT_API UBTTask_FaceGuardingDirection : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_FaceGuardingDirection(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AAICharacter* Guard;
};
