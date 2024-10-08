// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Tasks/BTTask_CheckSearchLocationProx.h"

#include "AILogic/AIGuardController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckSearchLocationProx::UBTTask_CheckSearchLocationProx(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = false;
	bNotifyTick = false;
	NodeName = "At Search Location"; 
}

EBTNodeResult::Type UBTTask_CheckSearchLocationProx::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector LocationToGo = OwnerComp.GetBlackboardComponent()->GetValueAsVector("SearchLocation");
	FVector CurrentLocation = Cast<AAIGuardController>(OwnerComp.GetAIOwner())->Agent->GetActorLocation(); 
	UE_LOG(LogTemp,Warning,TEXT("Distance To Point: %f"),FVector::Distance(LocationToGo,CurrentLocation) );

	if (FVector::Distance(LocationToGo,CurrentLocation) > 150.f)
	{
		return EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool("SearchStarted",true);
	APawn* Guard = OwnerComp.GetAIOwner()->K2_GetPawn();
	
	if(Guard->Implements<UEnemy>())
	{
		IEnemy::Execute_Bark(Guard,FString(TEXT("You can't hide! I will find you...."))); 
	}
	
	return EBTNodeResult::Failed; 
}
