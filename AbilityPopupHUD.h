// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityPopupHUD.generated.h" 

/**
 * 
 */
UCLASS(Abstract)
class PORTFOLIO_MECHANICS_API UAbilityPopupHUD: public UUserWidget
{
	GENERATED_BODY() 

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PopupText;

	void SetAbilityText(FString str);
	
	
};

