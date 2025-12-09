// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamGameMode.h"

#include "ShooterSamCharacter.h"
#include "ShooterSamPlayerController.h"
#include "Algo/RandomShuffle.h"
#include "EnemyAI/EnemySpawner.h"
#include "EnemyAI/ShooterAI.h"
#include "Kismet/GameplayStatics.h"

AShooterSamGameMode::AShooterSamGameMode()
{
	TimeSinceLastSpawnWave = 0.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterSamGameMode::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AShooterSamCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player)
	{
		Player->GameMode = this;
	}

	TArray<AShooterAI*> ShooterAIControllers;

	// TODO: Remove when all actors are spawned using enemySpawners
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), EnemyActors);

	for (AActor* ShooterAIActor : EnemyActors)
	{
		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterAIActor);
		ShooterAIActors.Add(ShooterAI);

		if (ShooterAI)
		{
			ShooterAI->StartBehaviourTree(Player);
		}
	}

	TArray<AActor*> EnemySpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), EnemySpawners);
	for (AActor* EnemySpawner : EnemySpawners)
	{
		AEnemySpawner* CombatEnemySpawner = Cast<AEnemySpawner>(EnemySpawner);
		CombatEnemySpawners.Add(CombatEnemySpawner);

		// SpawnEnemy(CombatEnemySpawner);
	}

	// if (ShooterAIActors.Num() > 0)
	// {
	// 	for (AActor* Enemy : ShooterAIActors)
	// 	{
	// 		ShooterAIControllers.Add(Cast<AShooterAI>(Enemy));
	// 	}
	//
	// 	for (AShooterAI* AIController : ShooterAIControllers)
	// 	{
	// 		AIController->StartBehaviourTree(Player);
	// 	}
	// }
}

void AShooterSamGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlayerHUD();
	
	TimeSinceLastSpawnWave += DeltaTime;

	if (TimeSinceLastSpawnWave > SpawnWaveIntervalSeconds)
	{
		SpawnEnemyWave();
		TimeSinceLastSpawnWave = 0;
	}
}

void AShooterSamGameMode::UpdatePlayerHUD()
{
	int32 NewInGameTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	int32 NewAILevel = CurrentAILevel;

	if (Player && Player->PlayerController && Player->PlayerController->HUDWidget)
	{
		if (NewInGameTime != InGameTime)
		{
			Player->PlayerController->HUDWidget->SetInGameTimeText(NewInGameTime);
			int32 AIExperience = GetSpawnedAIExperience();
			if (AIExperience >= AIExperienceNeededForLevelUp)
			{
				float NeededForCalculatedLevel;
				NewAILevel = GetLevelInfoFromExperience(AIExperience, NeededForCalculatedLevel, AIExperienceNeededForLevelUp);
			}
		}
		if (NewAILevel != CurrentAILevel)
		{
			Player->PlayerController->HUDWidget->SetAILevelText(NewAILevel);
		}
	}
	InGameTime = NewInGameTime;
	CurrentAILevel = NewAILevel;
}

float AShooterSamGameMode::GetSpawnedAIExperience()
{
	return (BaseExperienceNeededForLevelUp * FMath::Pow(XPMultiplierForNextLevel, static_cast<float>(InGameTime) / 60)) - BaseExperienceNeededForLevelUp;
}

/**
 * This function returns the level (of a character) based on an amount of experience. It also gives the threshold for reaching that level, and the threshold for reaching the next level.
 * @param CurrentExperienceAmount : The amount of experience the character has, based on which the level will be calculated.
 * @param ExperienceNeededForCalculatedLevel : The TOTAL (amount from level 1 to current level + 1) amount of experience needed to have reached the current level.
 * @param MaxExperienceCurrentLevel : The TOTAL  amount of experience needed to level up.
 * @return : The level the character has based on the CurrentExperienceAmount.
 */
int32 AShooterSamGameMode::GetLevelInfoFromExperience(float CurrentExperienceAmount,
                                                      float& ExperienceNeededForCalculatedLevel, float& MaxExperienceCurrentLevel) const
{
	int32 CalculatedLevel = 0;
	ExperienceNeededForCalculatedLevel = 0.0f;
	MaxExperienceCurrentLevel = 0.0f;
	while (true)
	{
		MaxExperienceCurrentLevel = BaseExperienceNeededForLevelUp * FMath::Pow(XPMultiplierForNextLevel, CalculatedLevel);
		
		if (CurrentExperienceAmount < ExperienceNeededForCalculatedLevel + MaxExperienceCurrentLevel)
		{
			break;
		}
		ExperienceNeededForCalculatedLevel += MaxExperienceCurrentLevel;
		CalculatedLevel++;
	}
	return CalculatedLevel + 1;
}

void AShooterSamGameMode::SpawnEnemyWave()
{
	Algo::RandomShuffle(CombatEnemySpawners);
	for (AEnemySpawner* Spawner : CombatEnemySpawners)
	{
		if (Spawner->GetCanSpawnEnemy())
		{
			SpawnEnemy(Spawner);
			break;
		}
	}
}

void AShooterSamGameMode::SpawnEnemy(AEnemySpawner* EnemySpawner)
{
	AShooterAI* Enemy = EnemySpawner->SpawnEnemy(Player, GetSpawnedAIExperience());
	if (Enemy)
	{
		Enemy->ThisCharacter->GameMode = this;
		ShooterAIActors.Add(Enemy);
	}
}

void AShooterSamGameMode::CharacterDied(AShooterSamCharacter* Character)
{
	if (Character == Player) return;
	for (AShooterAI* ShooterAI : ShooterAIActors)
	{
		if (Character == ShooterAI->ThisCharacter && ShooterAI->Spawner)
		{
			ShooterAI->Spawner->EnemyDied();
			break;
		}
	}
}
