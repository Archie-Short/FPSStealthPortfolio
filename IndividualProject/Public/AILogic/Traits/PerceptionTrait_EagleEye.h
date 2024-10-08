// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/Traits/PerceptionTraitBase.h"
#include "PerceptionTrait_EagleEye.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType,meta=(BlueprintSpawnableComponent))
class INDIVIDUALPROJECT_API UPerceptionTrait_EagleEye : public UPerceptionTraitBase
{
	GENERATED_BODY()
public:
	UPerceptionTrait_EagleEye(); 

	virtual void InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent) override;


};
