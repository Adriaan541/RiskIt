// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SHOOTERSAM_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* HealthBar;
	
	void SetHealthBar(float CurrentHealth, float MaxHealth);
};
