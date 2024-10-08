// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Tasks/BTTask_FaceGuardingDirection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AILogic/AIGuardController.h"
#include "AILogic/SmartObject.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FaceGuardingDirection::UBTTask_FaceGuardingDirection(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Face Guarding Direction"; 
}

EBTNodeResult::Type UBTTask_FaceGuardingDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIGuardController* Controller = Cast<AAIGuardController>(OwnerComp.GetAIOwner());
	Guard = Controller->Agent;
	if(!Controller || !Guard)
	{
		return EBTNodeResult::Failed; 
	}
	
	return EBTNodeResult::InProgress; 
}


void UBTTask_FaceGuardingDirection::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector GuardLookLocation = (Guard->DefaultBehaviour->FacingDirection->GetComponentLocation())+ (Guard->DefaultBehaviour->FacingDirection->GetForwardVector() * 100.f);
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Guard->GetActorLocation(),GuardLookLocation);
	FRotator LookAtBalanced = Guard->DefaultBehaviour->FacingDirection->GetComponentRotation();
	
	
	bool bSuccess = Guard->SetActorRotation(FMath::Lerp(Guard->GetActorRotation(),LookAtBalanced,0.05f),ETeleportType::None);
	float angle = abs(FMath::RadiansToDegrees(FVector::DotProduct(Guard->GetMesh()->GetForwardVector(),Guard->DefaultBehaviour->FacingDirection->GetForwardVector()))); 
	
	if(angle <= 1.f)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

	}
	

}