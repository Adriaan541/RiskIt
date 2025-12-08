// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterSamCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AShooterAI;

UCLASS()
class SHOOTERSAM_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterSamCharacter> ShooterSamClass;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* SpawnCapsule;

	TArray<AShooterAI*> ShooterAIActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CanSpawnEnemy = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCanSpawnEnemy() const {return CanSpawnEnemy;};

	int GetNumberOfEnemies();

	void EnemyDied();

	AShooterAI* SpawnEnemy(AShooterSamCharacter* Player, float EnemyExperience);

	void DestroyEnemy(AShooterAI* Enemy);
};
