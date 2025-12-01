// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamGameMode.h"

#include "ShooterSamCharacter.h"
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

	TArray<AShooterAI*> ShooterAIControllers;

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

		SpawnEnemy(CombatEnemySpawner);
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

	TimeSinceLastSpawnWave += DeltaTime;

	if (TimeSinceLastSpawnWave > SpawnWaveIntervalSeconds)
	{
		UE_LOG(LogTemp, Display, TEXT("Spawning wave"))
		SpawnEnemyWave();
		TimeSinceLastSpawnWave = 0;
	}
}

void AShooterSamGameMode::SpawnEnemyWave()
{
	for (AEnemySpawner* Spawner : CombatEnemySpawners)
	{
		if (Spawner->GetCanSpawnEnemy())
		{
			SpawnEnemy(Spawner);
		}
	}
}

void AShooterSamGameMode::SpawnEnemy(AEnemySpawner* EnemySpawner)
{
	AShooterAI* Enemy = EnemySpawner->SpawnEnemy(Player);
	if (Enemy)
	{
		Enemy->ThisCharacter->GameMode = this;
		ShooterAIActors.Add(Enemy);
	}
}

void AShooterSamGameMode::EnemyDied(AShooterSamCharacter* Enemy)
{
	for (AShooterAI* ShooterAI : ShooterAIActors)
	{
		if (Enemy == ShooterAI->ThisCharacter && ShooterAI->Spawner)
		{
			ShooterAI->Spawner->EnemyDied();
		}
	}
}
