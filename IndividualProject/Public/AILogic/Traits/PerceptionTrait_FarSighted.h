// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/Traits/PerceptionTraitBase.h"
#include "PerceptionTrait_FarSighted.generated.h"

/**
 * 
 */
UCLASS()
class INDIVIDUALPROJECT_API UPerceptionTrait_FarSighted : public UPerceptionTraitBase
{
	GENERATED_BODY()


public:
	UPerceptionTrait_FarSighted(); 

	virtual void InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent) override;


};
