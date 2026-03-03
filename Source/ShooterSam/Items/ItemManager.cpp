// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

#include "ItemType.h"
#include "LightningZap.h"
#include "ShooterSamPlayerController.h"
#include "Kismet/GameplayStatics.h"


UItemManager::UItemManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();
	UpdateItems();
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
		UItemBox* ItemBoxWidget = CreateWidget<UItemBox>(HUDWidget, ItemWidgetClass,
		                                                 WidgetNameFromItemName(Item.ItemName));
		if (ItemBoxWidget)
		{
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

void UItemManager::UpdateItemMap()
{
	TMap<EItemType, int> NewItemMap;
	for (auto item : Items)
	{
		NewItemMap.Add(item.ItemType, item.Amount);
	}
	ItemMap = NewItemMap;
}

void UItemManager::CreateWidgets()
{
	for (FItem& Item : Items)
	{
		UItemBox* ItemBoxWidget = CreateWidget<UItemBox>(HUDWidget, *ItemWidgetClass,
		                                                 WidgetNameFromItemName(Item.ItemName));
		if (ItemBoxWidget)
		{
			Item.ItemBox = ItemBoxWidget;
			ItemBoxWidget->Init(Item.Icon, Item.Amount);
		}
	}
}

FName UItemManager::WidgetNameFromItemName(FName ItemName)
{
	return FName(ItemName.ToString() + TEXT("Widget"));
}

void UItemManager::UpdateItems()
{
	UpdateItemMap();

	if (HUDWidget)
	{
		HUDWidget->UpdateItemBar(GetItems());
	}
}

TArray<FItem> UItemManager::GetItems()
{
	return Items;
}

FItem* UItemManager::GetItem(EItemType ItemType)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemType == ItemType)
			return &Item;
	}
	return nullptr;
}

TMap<EItemType, int> UItemManager::GetItemsMap()
{
	return ItemMap;
}

FItem UItemManager::GetRandomItem()
{
	return Items[FMath::RandRange(0, Items.Num() - 1)];
}

void UItemManager::AddItem(EItemType ItemType)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemType == ItemType)
		{
			Item.Amount = Item.Amount + 1;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItems();
}

void UItemManager::DeductItem(EItemType ItemType)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemType == ItemType)
		{
			Item.Amount--;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItems();
}

void UItemManager::SetItem(EItemType ItemType, int32 Amount)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemType == ItemType)
		{
			Item.Amount = Amount;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItems();
}

void UItemManager::RemoveItem(EItemType ItemType)
{
	for (FItem& Item : Items)
	{
		if (Item.ItemType == ItemType)
		{
			Item.Amount = 0;
			UpdateItemBoxWidget(Item);
			break;
		}
	}
	UpdateItems();
}

void UItemManager::OnHitProcChain(FProcChainData* ProcChainData)
{
	if (ProcChainData->IsChain()) 
			UE_LOG(LogTemp, Display, TEXT("Is procChain"));
	if (ItemMap[EItemType::LightningZap] > 0)
	{
		if (rand() % 5 < 2 || ProcChainData->IsChain())
		{
			ExecuteLightningZap(FProcChainData(*ProcChainData), ItemMap[EItemType::LightningZap]);
		}
	}
}

void UItemManager::ExecuteLightningZap(FProcChainData ProcChainData, int AmountOfItem)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ProcChainData.GetHitActor()->GetActorLocation());
	ALightningZap* LightningZap = GetWorld()->SpawnActorDeferred<ALightningZap>(LightningZapClass, SpawnTransform);

	if (LightningZap)
	{
		LightningZap->InitLightningZap(ProcChainData, AmountOfItem);
		UGameplayStatics::FinishSpawningActor(LightningZap, SpawnTransform);
	}
}
