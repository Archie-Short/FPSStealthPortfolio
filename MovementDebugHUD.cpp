// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementDebugHUD.h"

#include "Components/TextBlock.h"

void UMovementDebugHUD::TickUpdate(FVector PassedPosition,FRotator PassedPlayerRotation,float PassedCapsuleScale, float PassedHalfHeight, float PassedRadius,float PassedBobHeight, float PassedLeanAngle,FRotator PassedCameraRotation,EStance PassedStance, EMovementAbility PassedAbility,  EVaultingState PassedVaulting)
{
	if(PlayerPosition)
	{
		//Player Position
		PlayerPosition->SetText(FText::FromString(TEXT("Player Position: ") + PassedPosition.ToString()));
		
		//Player Rotation
		PlayerRotation->SetText(FText::FromString(TEXT("Player Rotation: ") + PassedPlayerRotation.ToString()));

		//Capsule Scale
		CapsuleScale->SetText(FText::FromString(TEXT("Capsule Scale: ") + FString::SanitizeFloat(PassedCapsuleScale)));

		//Capsule HalfHeight
		CapsuleHalfHeight->SetText(FText::FromString(TEXT("Capsule Half Height: ") + FString::SanitizeFloat(PassedHalfHeight)));

		//Capsule Radius
		CapsuleRadius->SetText(FText::FromString(TEXT("Capsule Radius: ") + FString::SanitizeFloat(PassedRadius)));

		//Camera Bob Height
		CameraBobHeight->SetText(FText::FromString(TEXT("Camera Bob Height: ") + FString::SanitizeFloat(PassedBobHeight)));

		//Camera Lean Angle
		CameraLeanAngle->SetText(FText::FromString(TEXT("Camera Lean Angle: ") + FString::SanitizeFloat(PassedLeanAngle)));

		//Camera Rotation
		CameraRotation->SetText(FText::FromString(TEXT("Camera Rotation: ") + PassedCameraRotation.ToString()));

		//Passed Stance
		PlayerStance->SetText(FText::FromString(TEXT("Character Stance: ") + UEnum::GetDisplayValueAsText(PassedStance).ToString()));

		//Passed Ability
		PlayerAbility->SetText(FText::FromString(TEXT("Character Movement Ability: ") + UEnum::GetDisplayValueAsText(PassedAbility).ToString()));
		
		//Passed Vaulting State
		PlayerVaultingStance->SetText(FText::FromString(TEXT("Character Vaulting: ") + UEnum::GetDisplayValueAsText(PassedVaulting).ToString()));
		 
		
		//PlayerPosition->SetText(FText::FromString(TEXT("Player Position: "))); 

	}
}

