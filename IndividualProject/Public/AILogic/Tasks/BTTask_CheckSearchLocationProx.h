// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckSearchLocationProx.generated.h"

/**
 * 
 */
UCLASS()
class INDIVIDUALPROJECT_API UBTTask_CheckSearchLocationProx : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_CheckSearchLocationProx(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	
};
