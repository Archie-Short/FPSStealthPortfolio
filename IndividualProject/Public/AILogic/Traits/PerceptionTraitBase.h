// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/Traits/TraitBase.h"
#include "PerceptionTraitBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType,meta=(BlueprintSpawnableComponent))
class INDIVIDUALPROJECT_API UPerceptionTraitBase : public UTraitBase
{
	GENERATED_BODY()

public:
	UPerceptionTraitBase(); 
	
	virtual void InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent){};

};
