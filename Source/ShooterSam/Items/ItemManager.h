// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManager.generated.h"


enum class EItemType : uint8;
class AGun;
class AShooterSamCharacter;
class ALightningZap;
class UHUDWidget;
class UItemBox;
class AShooterSamPlayerController;
struct FItemValues;

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY()
	int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	UPROPERTY()
	UItemBox* ItemBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* ItemMaterial;

	FItem(): ItemType()
	{
		ItemName = FName(NAME_None);
		Amount = 0;
		Icon = nullptr;
		ItemBox = nullptr;
		ItemMaterial = nullptr;
	}
};

USTRUCT(BluePrintType)
struct FProcChainData
{
	GENERATED_BODY()

	UPROPERTY()
	AGun* Instigator;
	UPROPERTY()
	AShooterSamCharacter* OwnerShooterSamCharacter;
	UPROPERTY()
	TArray<AShooterSamCharacter*> HitCharacters;
	UPROPERTY()
	float BaseDamage;

	FProcChainData()
	{
		Instigator = nullptr;
		OwnerShooterSamCharacter = nullptr;
		BaseDamage = 0;
	}

	FProcChainData(AGun* SetInstigator, AShooterSamCharacter* SetOwnerShooterSamCharacter,
								  AShooterSamCharacter* HitActor, float SetBaseDamage)
	{
		Instigator = SetInstigator;
		OwnerShooterSamCharacter = SetOwnerShooterSamCharacter;
		HitCharacters.Add(HitActor);
		BaseDamage = SetBaseDamage;
	}

	AShooterSamCharacter* GetHitActor()
	{
		return HitCharacters.Last();
	}

	AShooterSamCharacter* GetPreviousHit()
	{
		if (HitCharacters.Num() > 1)
		{
			return HitCharacters[HitCharacters.Num() - 2];
		}
		return nullptr;
	}

	bool IsChain() const
	{
		if (HitCharacters.Num() > 1) return true;
		return false;
	}

	bool IsNull()
	{
		if (Instigator == nullptr || OwnerShooterSamCharacter == nullptr || HitCharacters.Num() == 0) return true;
		return false;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTERSAM_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UHUDWidget* HUDWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemBox> ItemWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALightningZap> LightningZapClass;

	TArray<UUserWidget*> ItemWidgets;

	TMap<EItemType, int> ItemMap;

	FName WidgetNameFromItemName(FName ItemName);

	void UpdateItems();

	void CreateItemBoxWidget(FItem& Item);

	void UpdateItemBoxWidget(FItem& Item);

	void UpdateItemMap();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TArray<FItem> Items;

	TArray<FItem> GetItems();
	
	FItem* GetItem(EItemType ItemType);

	TMap<EItemType, int> GetItemsMap();

	FItem GetRandomItem();

	void Init(UHUDWidget* HUDWidgetReference);

	void CreateWidgets();

	void AddItem(EItemType ItemType);
	void DeductItem(EItemType ItemType);

	void SetItem(EItemType ItemType, int32 Amount);
	void RemoveItem(EItemType ItemType);

	// void OnHitProcChain(AGun* Instigator, AShooterSamCharacter* OwnerShooterSamCharacter,
	//                     AShooterSamCharacter* HitActor, float BaseDamage, AShooterSamCharacter* PreviousHit);
	void OnHitProcChain(FProcChainData* ProcChainData);
	void ExecuteLightningZap(FProcChainData ProcChainData, int AmountOfItem);

	void ExecuteLightningZap(FProcChainData ProcChainData);
};
