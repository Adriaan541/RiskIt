// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemySpawner.h"
#include "ShooterSamCharacter.h"
#include "ShooterAI.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERSAM_API AShooterAI : public AAIController
{
	GENERATED_BODY()

private:
	/*
	APawn* PlayerPawn;

	bool HadLineOfSight = false;
	
	FVector LastPlayerLocation;
	*/

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyAIBehaviourTree;

	AEnemySpawner* Spawner;

	AShooterSamCharacter* PlayerCharacter;
	AShooterSamCharacter* ThisCharacter;

	void StartBehaviourTree(AShooterSamCharacter* Player);
};
