// Fill out your copyright notice in the Description page of Project Settings.

#include "AILogic/Tasks/BTTask_GenerateSearchLocation.h"

#include "AIConfig.h"
#include "NavigationSystem.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GenerateSearchLocation::UBTTask_GenerateSearchLocation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = false;
	bNotifyTick = false;
	NodeName = "Generate Search Location"; 
}

EBTNodeResult::Type UBTTask_GenerateSearchLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UWorld* World = GetWorld(); 
	if(!Controller || !World)
	{
		return EBTNodeResult::Failed; 
	}
	
	FNavLocation ToGo;
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World); 

	while(true)
	{
		//Later replace radius to variable so map wide hunting can be done
		bool PointFound = NavSystem->GetRandomReachablePointInRadius(OwnerComp.GetBlackboardComponent()->GetValueAsVector("SearchLocation"),1000,ToGo);

		if(PointFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("MoveToLocation",ToGo.Location);
			return EBTNodeResult::Succeeded; 
	
		}
	}
	return EBTNodeResult::Failed; 
}
