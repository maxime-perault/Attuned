// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_InGameUI.generated.h"

/*
 * 
 */
UCLASS()
class ATTUNED_API UW_InGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Get")
		float GetCoolDown() const;

	UFUNCTION(BlueprintCallable, Category = "Set")
		void SetCoolDown(const float CoolDown);

	UFUNCTION(BlueprintCallable, Category = "Get")
		ESlateVisibility GetCoolDownVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "Set")
		void SetCoolDownVisibility(const ESlateVisibility CoolDown);
	
private:
	float				mv_CoolDown;
	ESlateVisibility	mv_CoolDownVisibility;
};
