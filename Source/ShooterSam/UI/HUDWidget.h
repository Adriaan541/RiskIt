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

USTRUCT(BlueprintType)
struct FItemWidgetsWithNames
{
	GENERATED_BODY()

	FName ItemName;
	UUserWidget* ItemWidget;
};

USTRUCT(BlueprintType)
struct FItemValues
{
	GENERATED_BODY()

	FString ItemName;
	int32 Amount;
};

UCLASS()
class SHOOTERSAM_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UHorizontalBox* ItemBar;

	TSubclassOf<UItemBox> ItemWidgetClass;

	TArray<UTexture2D*> ItemImages;

	TArray<FItemWidgetsWithNames> ItemWidgets;

	void SetHealthBar(float CurrentHealth, float MaxHealth);

	void UpdateItemBar(TArray<FItem> Items);

	void UpdateItems(TArray<FItemValues> Items);

	FName WidgetNameFromItemName(FString ItemName);

	void CreateItemWidget(FItemValues Item);
};
