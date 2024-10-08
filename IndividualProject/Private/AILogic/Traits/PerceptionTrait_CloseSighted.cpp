// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Traits/PerceptionTrait_CloseSighted.h"

UPerceptionTrait_CloseSighted::UPerceptionTrait_CloseSighted()
{
	Title = FText::FromString(TEXT("Poor Vision"));
	Description = FText::FromString(TEXT("Their eyes aren't as focused as the average, the distance they can see is greatly reduced"));

}

void UPerceptionTrait_CloseSighted::InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,
	UAIPerceptionComponent* HearingComponent)
{
	Super::InitialisePerceptionTrait(VisionComponent, HearingComponent);

	VisionComponent->FocVisLength = VisionComponent->FocVisLength /2;
	VisionComponent->PeriphVisLength = VisionComponent->PeriphVisLength /2;


}
