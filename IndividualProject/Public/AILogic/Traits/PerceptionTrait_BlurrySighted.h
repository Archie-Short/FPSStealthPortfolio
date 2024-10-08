// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/Traits/PerceptionTraitBase.h"
#include "PerceptionTrait_BlurrySighted.generated.h"

/**
 * 
 */
UCLASS()
class INDIVIDUALPROJECT_API UPerceptionTrait_BlurrySighted : public UPerceptionTraitBase
{
	GENERATED_BODY()
	
	public:
    	UPerceptionTrait_BlurrySighted(); 
    
    	virtual void InitialisePerceptionTrait(UAI_VisionComponent* VisionComponent,UAIPerceptionComponent* HearingComponent) override;

};
