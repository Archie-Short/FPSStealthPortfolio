// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGuardController.h"
#include "GameFramework/Actor.h"
#include "GuardOverwatch.generated.h"

UCLASS()
class INDIVIDUALPROJECT_API AGuardOverwatch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuardOverwatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<AAIGuardController*> LevelAIControllers; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddDeadGuardToWatch(AAICharacter* guard); 

};
