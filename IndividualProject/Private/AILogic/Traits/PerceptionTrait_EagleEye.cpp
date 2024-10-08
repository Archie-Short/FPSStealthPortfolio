// Fill out your copyright notice in the Description page of Project Settings.


#include "AILogic/Traits/PerceptionTrait_EagleEye.h"

UPerceptionTrait_EagleEye::UPerceptionTrait_EagleEye():UPerceptionTraitBase()
{
	Title = FText::FromString(TEXT("Eagle Eye"));
	Description = FText::FromString(TEXT("With keen eyes this person can see much clearer than average, even in their peripheral vision they will spot you very quickly "));

}

void UPerceptionTrait_EagleEye::InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent)
{
	Super::InitialisePerceptionTrait(VisionComponent,HearingComponent);

	//Set the focused vision length to the peripheral vision length 
	VisionComponent->FocVisLength = VisionComponent->PeriphVisLength;
	VisionComponent->FocVisHeight = VisionComponent->PeriphVisHeight; 
	VisionComponent->FocVisWidth = VisionComponent->PeriphVisWidth;

	UE_LOG(LogTemp,Warning,TEXT("Correct Method Called"));


}
