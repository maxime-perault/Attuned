// Fill out your copyright notice in the Description page of Project Settings.

#include "W_InGameUI.h"



float UW_InGameUI::GetCoolDown() const
{
	return (mv_CoolDown);
}

void UW_InGameUI::SetCoolDown(const float CoolDown)
{
	mv_CoolDown = CoolDown;
}

ESlateVisibility UW_InGameUI::GetCoolDownVisibility() const
{
	return (mv_CoolDownVisibility);
}

void UW_InGameUI::SetCoolDownVisibility(const ESlateVisibility CoolDownVisibility)
{
	mv_CoolDownVisibility = CoolDownVisibility;
}