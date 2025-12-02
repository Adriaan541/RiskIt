// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManager.generated.h"


class UHUDWidget;
class UItemBox;
class AShooterSamPlayerController;
struct FItemValues;

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

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

	FItem()
	{
		ItemName = FName(NAME_None);
		Amount = 0;
		Icon = nullptr;
		ItemBox = nullptr;
		ItemMaterial = nullptr;
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

	TArray<UUserWidget*> ItemWidgets;

	FName WidgetNameFromItemName(FName ItemName);

	void UpdateItemBar();
	
	void CreateItemBoxWidget(FItem& Item);
	
	void UpdateItemBoxWidget(FItem& Item);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	TArray<FItem> Items;

	TArray<FItem> GetItems();

	FItem GetRandomItem();

	void Init(UHUDWidget* HUDWidgetReference) ;
	
	void CreateWidgets();

	void AddItem(FName ItemName);
	void DeductItem(FString ItemName);

	void SetItem(FString ItemName, int32 Amount);
	void RemoveItem(FString ItemName);
};
