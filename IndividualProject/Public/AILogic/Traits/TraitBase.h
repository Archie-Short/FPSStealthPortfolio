// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/AI_VisionComponent.h"
#include "TraitBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETraitType : uint8
{
	EPerception UMETA(DisplayName = "Perception"),
	ERelationship UMETA(DisplayName = "Relationship"),
	EEnvironmental UMETA(DisplayName = "Environmental"),
	ETenacity UMETA(DisplayName ="Tenacity"),

};

UCLASS(Blueprintable, BlueprintType,meta=(BlueprintSpawnableComponent))
class INDIVIDUALPROJECT_API UTraitBase : public UObject 
{
	GENERATED_BODY()
public:
	UTraitBase();
	~UTraitBase();

	virtual void InitialiseTrait()
	{
	}

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	ETraitType TraitType;


	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Title;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Description; 
	
};
