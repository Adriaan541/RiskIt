// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

#include "ShooterSamPlayerController.h"


UItemManager::UItemManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();
}

void UItemManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UItemManager::Init(UHUDWidget* HUDWidgetReference)
{
	HUDWidget = HUDWidgetReference;
	for (FItem& Item : Items)
	{
		Item.Amount = 0;
		CreateItemBoxWidget(Item);
	}
}

void UItemManager::CreateItemBoxWidget(FItem& Item)
{
	if (HUDWidget)
	{
		UItemBox* ItemBoxWidget = CreateWidget<UItemBox>(HUDWidget, ItemWidgetClass, WidgetNameFromItemName(Item.ItemName));
		UE_LOG(LogTemp, Display, TEXT("Creating item widget"));
		if (ItemBoxWidget)
		{
			UE_LOG(LogTemp, Display, TEXT("Item Widget Created"));
			Item.ItemBox = ItemBoxWidget;
			ItemBoxWidget->Init(Item.Icon, Item.Amount);
		}
	}
}

void UItemManager::UpdateItemBoxWidget(FItem& Item)
{
	if (Item.ItemBox)
	{
		Item.ItemBox->SetAmount(Item.Amount);
	}
}

void UItemManager::CreateWidgets()
{
	for (FItem& Item : Items)
	{
		UItemBox* ItemBoxWidget = CreateWidget<UItemBox>(HUDWidget, *ItemWidgetClass, WidgetNameFromItemName(Item.ItemName));
		UE_LOG(LogTemp, Display, TEXT("Creating item widget"));
		if (ItemBoxWidget)
		{
			UE_LOG(LogTemp, Display, TEXT("Item Widget Created"));
			Item.ItemBox = ItemBoxWidget;
			ItemBoxWidget->Init(Item.Icon, Item.Amount);
		}
	}
}

FName UItemManager::WidgetNameFromItemName(FName ItemName)
{
	return FName(ItemName.ToString() + TEXT("Widget"));
}

void UItemManager::UpdateItemBar()
{
	if (HUDWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("Item Bar Updated"));
		UE_LOG(LogTemp, Display, TEXT("Items: %i"), GetItems().Num());
		HUDWidget->UpdateItemBar(GetItems());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("HUDWidget null"));
	}
}

TArray<FItem> UItemManager::GetItems()
{
	return Items;
}

FItem UItemManager::GetRandomItem()
{
	return Items[FMath::RandRange(0, Items.Num() - 1)];
}

void UItemManager::AddItem(FName ItemName)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemName == ItemName)
		{
			Item.Amount = Item.Amount + 1;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItemBar();
}

void UItemManager::DeductItem(FString ItemName)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemName == ItemName)
		{
			Item.Amount--;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItemBar();
}

void UItemManager::SetItem(FString ItemName, int32 Amount)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemName == ItemName)
		{
			Item.Amount = Amount;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItemBar();
}

void UItemManager::RemoveItem(FString ItemName)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemName == ItemName)
		{
			Item.Amount = 0;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItemBar();
}

