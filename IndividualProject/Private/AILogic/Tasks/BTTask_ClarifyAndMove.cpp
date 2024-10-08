// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Tasks/BTTask_ClarifyAndMove.h"
#include "AILogic/AIGuardController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClarifyAndMove::UBTTask_ClarifyAndMove(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Clarify Last Location"; 
}

EBTNodeResult::Type UBTTask_ClarifyAndMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIGuardController* Controller = Cast<AAIGuardController>(OwnerComp.GetAIOwner());
	Guard = Controller->Agent;
	if(!Controller || !Guard)
	{
		return EBTNodeResult::Failed; 
	}
	
	return EBTNodeResult::InProgress; 
}


void UBTTask_ClarifyAndMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector LocationToLook = OwnerComp.GetBlackboardComponent()->GetValueAsVector("LastPlayerLocation");
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Guard->GetActorLocation(),LocationToLook);
	FRotator LookAtBalanced = FRotator(0,LookAt.Yaw,0);
	
	bool bSuccess = Guard->SetActorRotation(FMath::Lerp(Guard->GetActorRotation(),LookAtBalanced,0.05f),ETeleportType::None);
	float angle = FMath::RadiansToDegrees(FVector::DotProduct(Guard->GetMesh()->GetForwardVector(),(LocationToLook - Guard->GetActorLocation()).GetUnsafeNormal())); 
	
	UE_LOG(LogTemp,Warning,TEXT("Angle Left To Rotate: %f"),angle)
	if(abs(angle) < 1.f)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

	}
	//if(false)
	//{
	//	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	//}
}
