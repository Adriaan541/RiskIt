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

TArray<FItemValues> UItemManager::GetItems()
{
	return Items;
}

void UItemManager::AddItem(FString ItemName)
{
	for (FItemValues& Item : Items)
	{
		if (ItemName == Item.ItemName)
		{
			Item.Amount++;
			return;
		}
	}
	Items.Add({ItemName, 1});
}

void UItemManager::DeductItem(FString ItemName)
{
	for (FItemValues& Item : Items)
	{
		if (ItemName == Item.ItemName)
		{
			Item.Amount--;
			return;
		}
	}
}

void UItemManager::SetItem(FString ItemName, int32 Amount)
{
}

void UItemManager::RemoveItem(FString ItemName)
{
}

