// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterSamGameMode.generated.h"

class AEnemySpawner;
class AShooterSamCharacter;
class AShooterAI;
/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AShooterSamGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AShooterSamCharacter* Player;
	
	TArray<AShooterAI*> ShooterAIActors;

	TArray<AEnemySpawner*> CombatEnemySpawners;

	UPROPERTY(VisibleAnywhere)
	float TimeSinceLastSpawnWave;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SpawnEnemyWave();

	void SpawnEnemy(AEnemySpawner* EnemySpawner);

	

public:
	AShooterSamGameMode();

	void EnemyDied(AShooterSamCharacter* Enemy);

	UPROPERTY(EditAnywhere)
	float SpawnWaveIntervalSeconds = 60.0f;
};



