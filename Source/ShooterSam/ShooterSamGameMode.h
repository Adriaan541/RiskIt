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

	UPROPERTY(EditAnywhere)
	float BaseExperienceNeededForLevelUp = 100.0f;

	UPROPERTY(EditAnywhere)
	float XPMultiplierForNextLevel = 1.2f;

	UPROPERTY(EditAnywhere)
	float AIExperienceGainedMultiplier = 1.05f;

	int32 InGameTime = 0;

	UPROPERTY(VisibleAnywhere)
	float TimeSinceLastSpawnWave;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SpawnEnemyWave();

	void SpawnEnemy(AEnemySpawner* EnemySpawner);

public:
	AShooterSamGameMode();

	float GetSpawnedAIExperience();
	
	int32 GetLevelInfoFromExperience(float CurrentExperienceAmount, float &ExperienceNeededForCalculatedLevel, float &MaxExperienceCurrentLevel) const;

	void CharacterDied(AShooterSamCharacter* Character);

	UPROPERTY(EditAnywhere)
	float SpawnWaveIntervalSeconds = 60.0f;
};



