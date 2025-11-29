// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManager.generated.h"


class UItemBox;
class AShooterSamPlayerController;
struct FItemValues;

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

	TArray<UItemBox*> ItemWidgets;

	TArray<FItemValues> Items;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FItemValues> GetItems();

	void AddItem(FString ItemName);
	void DeductItem(FString ItemName);

	void SetItem(FString ItemName, int32 Amount);
	void RemoveItem(FString ItemName);
};
