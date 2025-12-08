// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "HUDWidget.generated.h"

struct FItem;
class UImage;
class UScaleBox;
class UTextBlock;
/**
 * 
 */

UCLASS()
class SHOOTERSAM_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UHorizontalBox* ItemBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* ExperienceBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* CurrentLevelText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* HealthText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* EnemyLevelText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* GameTimerText;
	
	void UpdateItemBar(TArray<FItem> Items);

	void SetExperienceBar(int32 CurrentLevel, float CurrentExperience, float MaxExperience);

	void SetHealthBar(float CurrentHealth, float MaxHealth);

	void SetAILevelText(int32 Level);

	void SetInGameTimeText(int32 TimeSeconds);
};
