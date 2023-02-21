// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityPopupHUD.h"

#include "Components/TextBlock.h"

void UAbilityPopupHUD::SetAbilityText(FString str)
{
	PopupText->SetText(FText::FromString(str)); 
}
