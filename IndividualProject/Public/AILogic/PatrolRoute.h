// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AILogic/SmartObject.h"
#include "PatrolRoute.generated.h"

/**
 * 
 */
UCLASS()
class INDIVIDUALPROJECT_API APatrolRoute : public ASmartObject
{
	GENERATED_BODY()

public:
	APatrolRoute();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SplinePath")
	class USplineComponent* Path;

	void GetSplinePoints();

	TArray<FVector> Locations; 
};
