// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamGameMode.h"

#include "ShooterSamCharacter.h"
#include "EnemyAI/ShooterAI.h"
#include "Kismet/GameplayStatics.h"

void AShooterSamGameMode::BeginPlay()
{
	Super::BeginPlay();

	AShooterSamCharacter* Player = Cast<AShooterSamCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TArray<AShooterAI*> ShooterAIControllers;

	TArray<AActor*> ShooterAIActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), ShooterAIActors);

	for (AActor* ShooterAIActor : ShooterAIActors)
	{
		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterAIActor);

		if (ShooterAI)
		{
			ShooterAI->StartBehaviourTree(Player);
		}
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

AShooterSamGameMode::AShooterSamGameMode()
{
	// stub
}
