// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EAIState : uint8
{
	EIdle UMETA(DisplayName = "Idle"),
	EDefault UMETA(DisplayName = "Default"),
	EClarify UMETA(DisplayName = "Clarify"),
	EAlert UMETA(DisplayName ="Alert"),
	ESearch UMETA(DisplayName = "Search"),
	EInvestigate UMETA(DisplayName = "Investigate"), 
	EEngage UMETA(DisplayName = "Engage"),
};