// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Components/ScaleBox.h"
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

void UHUDWidget::UpdateItems(TArray<FItemValues> Items)
{
	for (FItemValues Item : Items)
	{
		UUserWidget* ItemBoxWidget = CreateWidget(this, *ItemWidgetClass, FName(*Item.ItemName));
		if (ItemBoxWidget && ItemBar)
		{
			ItemWidgets.Add(ItemBoxWidget);
			ItemBar->AddChild(ItemBoxWidget);
			UItemBox* ItemBox = Cast<UItemBox>(ItemBoxWidget);
			ItemBox->Init(ItemImages[0], Item.Amount);
		}
	}
}


// void UHUDWidget::UpdateItems(TArray<FItemValues> Items)
// {
// 	TArray<TPair<FString, UUserWidget*>> ItemWidgetsWithNames;
// 	for (UUserWidget* ItemWidget : ItemWidgets)
// 	{
// 		ItemWidgetsWithNames.Add({ItemWidget->GetName(), ItemWidget});
// 	}
// 	
// 	for (FItemValues Item : Items)
// 	{
// 		FString WidgetName = FString::Printf(TEXT("%sBox"), *Item.ItemName);
// 		if (ItemWidgetsWithNames[WidgetName])
// 		{
// 			
// 		}
// 		
// 		UItemBox* ItemBox = Cast<UItemBox>(CreateWidgetInstance(*ItemBar, UItemBox::StaticClass(), FName(*Item)));
// 		
// 		
// 		
// 		ItemBox->SetItemAndAmount(ItemImages.FindItemByClass(FName(TEXT("%s Icon"), )));
// 		
// 		ItemWidgets.Add(CreateWidgetInstance(*ItemBar, UItemBox::StaticClass(), FName(*Item)));
// 	}
// }


/* TODO: Add Item functionality
 * void UpdateItems()
 */
