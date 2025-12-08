// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"

#include "Components/ProgressBar.h"

void UEnemyHealthBar::SetHealthBar(float CurrentHealth, float MaxHealth)
{
	float HealthDecimal;

	if (CurrentHealth > MaxHealth)
	{
		HealthDecimal = 1.0f;
	}
	else if (CurrentHealth <= MaxHealth && CurrentHealth >= 0)
	{
		HealthDecimal = CurrentHealth / MaxHealth;
	}
	else
	{
		HealthDecimal = 0.0f;
	}

	HealthBar->SetPercent(HealthDecimal);
}
