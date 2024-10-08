// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Traits/PerceptionTrait_BlurrySighted.h"


UPerceptionTrait_BlurrySighted::UPerceptionTrait_BlurrySighted():UPerceptionTraitBase()
{
	Title = FText::FromString(TEXT("Blurry Vision"));
	Description = FText::FromString(TEXT("Their eyes not as focused as the average, even in direct line of sight they will detect you as if you are at the edge of their peripheral "));

}

void UPerceptionTrait_BlurrySighted::InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent)
{
	Super::InitialisePerceptionTrait(VisionComponent,HearingComponent);

	//Set the focused vision length to the peripheral vision length 
	VisionComponent->FocVisLength = 0;
	VisionComponent->FocVisHeight = 0;
	VisionComponent->FocVisWidth = 0; 
	
	UE_LOG(LogTemp,Warning,TEXT("Correct Method Called"));
}
