// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_InformationComponent.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "AILogic/AI_VisionComponent.h"
#include "AICharacter.generated.h"

enum class EAIState : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS()
class INDIVIDUALPROJECT_API AAICharacter : public ACharacter, public IEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AI")
	class UBehaviorTree* TreeAsset;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AI")
	class ASmartObject* DefaultBehaviour;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	UAI_InformationComponent* AIInformationComponent; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateAwareness(float AwarenessUpdate);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= "AI")
	float GuardAwareness;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	EAIState StartState;

	//Speed Control
	void SetMovementSpeed(float NewSpeed); 

	//On Death
	
	FOnDeathSignature OnDeathDelegate;
	
	//Interface functions
	virtual void Die_Implementation() override;
	
	virtual float AngleFromFacing_Implementation(FVector PlayerLocation) override;

	virtual bool GetDeathState_Implementation() override;

	virtual UAI_InformationComponent* GetAIInformation_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loot")
	void Bark(const FString &ToSay);
	
	//virtual void Bark_Implementation(const FString &ToSay) override;

	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool IsAlive;
	

};
