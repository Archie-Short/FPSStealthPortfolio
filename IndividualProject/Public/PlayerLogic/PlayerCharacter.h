// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerLogic/PlayerCharacter_MoveComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class INDIVIDUALPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	//Movement Component
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Movement")
	UPlayerCharacter_MoveComponent* StealthMovementComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	//Crouching 
	void CrouchInput();

	//Blueprint crouching for easier animation handling
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category="Movement")
	void BlueprintCrouchToggle(); 
	
	//Leaning Functions 
	void LeanPressed();

	void LeanReleased();

	//Jumping Functions
	void CheckJump();

	//Sprinting Functions
	void StartSprinting();

	void StopSprinting();

	void VerticalCameraInput(float Val);

	UFUNCTION(BlueprintCallable)
	void PrimaryActionPressed();

	void LeftMouseReleased();
	
	void Distract(); 

	//RaycastParameters
	FCollisionQueryParams TraceParams; 

	//Backstab Variables

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="Backstab")
	void UpdateCanBackstab(bool CanBackstab);


	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,Category="UI")
	void UpdateEnemyInView(bool InView);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bEnemyInView = false;

	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bCanBackStab = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* LookAtEnemy; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector BackstabLocation;


	//Hearing
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= "Sound")
	UAIPerceptionStimuliSourceComponent* stimulus;

	//Sounds
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Sound")
	USoundBase* Clap;

	
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	



};


