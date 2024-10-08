// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_InformationComponent.h"
#include "Enemy.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class  UEnemy : public UInterface
{
	GENERATED_BODY()
};

class INDIVIDUALPROJECT_API IEnemy{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Bark(const FString &ToSay);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAI_InformationComponent* GetAIInformation();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	float AngleFromFacing(FVector PlayerLocation);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool GetDeathState();


};
