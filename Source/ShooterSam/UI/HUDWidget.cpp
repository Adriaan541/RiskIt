// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "ItemManager.h"
#include "Components/TextBlock.h"

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

/* TODO: Add Item functionality
 * void UpdateItems()
 */
