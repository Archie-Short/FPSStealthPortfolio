// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Traits/PerceptionTrait_FarSighted.h"

UPerceptionTrait_FarSighted::UPerceptionTrait_FarSighted()
{
Title = FText::FromString(TEXT("Far Sighted"));
	Description = FText::FromString(TEXT("Equip with a shiny pair of new glasses this person can see much further than average, be wary through long hallways and open spaces"));

}

void UPerceptionTrait_FarSighted::InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,
	UAIPerceptionComponent* HearingComponent)
{
	Super::InitialisePerceptionTrait(VisionComponent, HearingComponent);

	VisionComponent->FocVisLength = VisionComponent->FocVisLength *2;
	VisionComponent->PeriphVisLength = VisionComponent->PeriphVisLength *2;

	
}
