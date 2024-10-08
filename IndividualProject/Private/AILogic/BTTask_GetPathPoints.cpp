// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/BTTask_GetPathPoints.h"
#include "AILogic/AIGuardController.h"
#include "Components/SplineComponent.h"
#include "AILogic/AICharacter.h"
#include "AILogic/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetPathPoints::UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points"; 
}

EBTNodeResult::Type UBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	AAIController* Controller = OwnerComp.GetAIOwner();
	if(!Controller || !BlackBoard)
	{
		return EBTNodeResult::Failed; 
	}
	AAICharacter* Guard = Cast<AAICharacter>(Controller->GetPawn());
	if(!Guard)
	{
		return EBTNodeResult::Failed; 
	}
	APatrolRoute* PathRef = Cast<APatrolRoute>(Guard->DefaultBehaviour);
	if(!PathRef || PathRef->Locations.Num() < 1)
	{
		return EBTNodeResult::Succeeded; 
	}
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation",PathRef->Locations[Index]);

	if(Index < PathRef->Locations.Num() -1)
	{
		Index++;
		return EBTNodeResult::Succeeded; 
 
	}
	Index = 0;
	return EBTNodeResult::Succeeded; 
}

