// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VaultingState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EVaultingState
{
	Vaulst_Not_Vaulting	UMETA(DisplayName = "Not Vaulting"),
	Vaulst_Can_Vault	UMETA(DisplayName = "Can Vault"),
	Vaulst_Vaulting		UMETA(DisplayName = "Vaulting")
};