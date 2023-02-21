
#pragma once

#include "CoreMinimal.h"
#include "StanceState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EStance
{
	//0
	EStanceStanding	UMETA(DisplayName = "Standing"),
	//1
	EStanceAttemptingCrouch UMETA(DisplayName = "Attempting Crouch"),
	//2
	EStanceAttemptingStand UMETA(DisplayName = "Attempting Stand"),
	//3
	EStanceCrouched	UMETA(DisplayName = "Crouching"),
	//4
	EStanceAttemptingStoop UMETA(DisplayName = "Attempting Stoop"),
	//5
	EStanceAttemptingStandFromStoop UMETA(DisplayName = "Attempting Stand From Stoop"),
	//6
	EStanceStooped		UMETA(DisplayName = "Stooping"),
	//7
	EStanceStoopedUnderObject UMETA(DisplayName ="Stooped Under Object"),
	//8
	EStanceAttemptingStoopLower UMETA(DisplayName, "Attempting Stoop Lower"),
	//9
	EStanceAttemptingStoopHigher UMETA(DisplayName, "Attempting Stoop Higher")

};

UENUM(BlueprintType)
enum EMovementAbility
{
	EmaNeutral UMETA(DisplayName = "Neutral"),
	EmaLeaning UMETA(DisplayName = "Leaning"),
	EmaReturningFromLean UMETA(DisplayName = "Returning From Lean"),
	EmaSprinting UMETA(DisplayName = "Sprinting")
};
