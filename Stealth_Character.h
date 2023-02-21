// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Stealth_Movement_Component.h"
#include "Stealth_Character.generated.h"


UCLASS()
class PORTFOLIO_MECHANICS_API AStealth_Character : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	AStealth_Character();

	//Movement Component
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Movement")
	UStealth_Movement_Component* StealthMovementComponent; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FCollisionQueryParams RaycastCollisionParams;

	//Blink Variables
	bool bBlinkHeld = true; 


	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	void CrouchInput();
	
	//Leaning Functions 
	void LeanPressed();

	void LeanReleased();

	//Jumping Functions
	void CheckJump();

	//Sprinting Functions
	void StartSprinting();

	void StopSprinting();

	void VerticalCameraInput(float Val);

	void LeftMousePressed();

	void LeftMouseReleased(); 
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


};
