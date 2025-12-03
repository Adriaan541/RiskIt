// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Items/ItemManager.h"
#include "Components/TextBlock.h"



void UHUDWidget::UpdateItemBar(TArray<FItem> Items)
{
	for (FItem Item : Items)
	{
		if (ItemBar && Item.ItemBox)
		{
			bool ItemIsInItemBar = Item.ItemBox->GetParent() == ItemBar;
			if (ItemIsInItemBar && Item.Amount <= 0)
			{
				ItemBar->RemoveChild(Item.ItemBox);
			}
			if (!ItemIsInItemBar && Item.Amount > 0)
			{
				ItemBar->AddChild(Item.ItemBox);
			}
		}
	}
}

void UHUDWidget::SetExperienceBar(int32 CurrentLevel, float CurrentExperience, float MaxExperience)
{
	float ExperienceDecimal;
	FText CurrentLevelTextString;

	if (CurrentExperience > MaxExperience)
	{
		ExperienceDecimal = 1.0f;
	}
	else if (CurrentExperience <= 0.0f)
	{
		ExperienceDecimal = 0.0f;
	}
	else
	{
		ExperienceDecimal = CurrentExperience / MaxExperience;
	}

	ExperienceBar->SetPercent(ExperienceDecimal);
	CurrentLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %i"), CurrentLevel)));
}

void UHUDWidget::SetHealthBar(float CurrentHealth, float MaxHealth)
{
	float HealthDecimal;
	FText HealthTextString;

	if (CurrentHealth > MaxHealth)
	{
		HealthTextString = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth));
		HealthDecimal = 1.0f;
	}
	else if (CurrentHealth <= MaxHealth && CurrentHealth >= 0)
	{
		HealthTextString = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth));
		HealthDecimal = CurrentHealth / MaxHealth;
	}
	else
	{
		HealthTextString = FText::FromString(FString::Printf(TEXT("0 / %.0f"), MaxHealth));
		HealthDecimal = 0.0f;
	}

	HealthBar->SetPercent(HealthDecimal);
	HealthText->SetText(HealthTextString);
}

void UHUDWidget::SetAILevelText(int32 Level)
{
	EnemyLevelText->SetText(FText::FromString(FString::Printf(TEXT("AI Level: %i"), Level)));
}

void UHUDWidget::SetInGameTimeText(int32 TimeSeconds)
{
	int32 Hours = TimeSeconds / 3600;
	int32 minutes = TimeSeconds / 60;
	int32 seconds = TimeSeconds % 60;
	FText TimerTextString;
	if (Hours > 0)
	{
		TimerTextString = FText::FromString(FString::Printf(TEXT("Time: %02i:%02i:%02i"), Hours, minutes, seconds));
	}
	else
	{
		TimerTextString = FText::FromString(FString::Printf(TEXT("Time: %02i:%02i"), minutes, seconds));
	}
	GameTimerText->SetText(TimerTextString);
}

/* TODO: Add Item functionality
 * void UpdateItems()
 */
