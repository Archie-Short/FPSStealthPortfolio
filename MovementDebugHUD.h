// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StanceState.h"
#include "VaultingState.h"
#include "MovementDebugHUD.generated.h" 
/**
 * 
 */
UCLASS(Abstract)
class PORTFOLIO_MECHANICS_API UMovementDebugHUD : public UUserWidget
{
	GENERATED_BODY() 
public:
	void TickUpdate(FVector PassedPosition,FRotator PassedPlayerRotation,float PassedCapsuleScale, float PassedHalfHeight, float PassedRadius,float PassedBobHeight, float PassedLeanAngle,FRotator PassedCameraRotation,EStance PassedStance, EMovementAbility PassedAbility,  EVaultingState PassedVaulting);

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayerPosition;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayerRotation;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CapsuleScale;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CapsuleHalfHeight;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CapsuleRadius;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CameraBobHeight;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CameraLeanAngle;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* CameraRotation;

	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayerStance;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayerVaultingStance;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayerAbility; 
};
