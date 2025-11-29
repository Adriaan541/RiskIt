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
	UE_LOG(LogTemp, Display, TEXT("Item Bar Updated"));
	for (FItem Item : Items)
	{
		if (ItemBar && Item.ItemBox)
		{
			UE_LOG(LogTemp, Display, TEXT("Itembox found"));
			bool ItemIsInItemBar = Item.ItemBox->GetParent() == ItemBar;
			if (ItemIsInItemBar && Item.Amount <= 0)
			{
				ItemBar->RemoveChild(Item.ItemBox);
			}
			if (!ItemIsInItemBar && Item.Amount > 0)
			{
				UE_LOG(LogTemp, Display, TEXT("Adding child"));
				ItemBar->AddChild(Item.ItemBox);
			}
		}
	}
}

void UHUDWidget::UpdateItems(TArray<FItemValues> Items)
{
	UE_LOG(LogTemp, Display, TEXT("Item Bar Updated"));
	for (FItemValues& Item : Items)
	{
		FItemWidgetsWithNames* ItemWidgetWithName = ItemWidgets.FindByPredicate([&](const FItemWidgetsWithNames& Entry)
		{
			return Entry.ItemName == Item.ItemName;
		});
		if (ItemWidgetWithName)
		{
			UItemBox* ItemBox = Cast<UItemBox>(ItemWidgetWithName->ItemWidget);
			if (ItemBox)
			{
				ItemBox->SetAmount(Item.Amount);
				continue;
			}
		}
		CreateItemWidget(Item);
	}
}

FName UHUDWidget::WidgetNameFromItemName(FString ItemName)
{
	return FName(FString::Printf(TEXT("%sWidget"), *ItemName));
}



void UHUDWidget::CreateItemWidget(FItemValues Item)
{
	UUserWidget* ItemBoxWidget = CreateWidget(this, *ItemWidgetClass, WidgetNameFromItemName(*Item.ItemName));
	if (ItemBoxWidget && ItemBar)
	{
		ItemWidgets.Add({WidgetNameFromItemName(*Item.ItemName), ItemBoxWidget});
		UItemBox* ItemBox = Cast<UItemBox>(ItemBoxWidget);
		ItemBox->Init(ItemImages[0], Item.Amount);
		ItemBar->AddChild(ItemBoxWidget);
	}
}


/* TODO: Add Item functionality
 * void UpdateItems()
 */
