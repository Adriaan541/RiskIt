// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemySpawner.h"

#include "ShooterAI.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SpawnCapsule"));
	SpawnCapsule->SetupAttachment(RootComponent);

	SpawnCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int AEnemySpawner::GetNumberOfEnemies()
{
	return ShooterAIActors.Num();
}

void AEnemySpawner::EnemyDied()
{
	CanSpawnEnemy = true;
}

AShooterAI* AEnemySpawner::SpawnEnemy(AShooterSamCharacter* Player, float EnemyExperience)
{
	if (GetNumberOfEnemies() >= 3)
	{
		DestroyEnemy(ShooterAIActors[0]);
	}
	
	FTransform SpawnTransform(FVector(SpawnCapsule->GetComponentLocation()));
	AShooterSamCharacter* ShooterSamCharacter = GetWorld()->SpawnActorDeferred<AShooterSamCharacter>(
		ShooterSamClass, SpawnTransform);

	ShooterSamCharacter->CurrentExperience = EnemyExperience;
	ShooterSamCharacter->AutoPossessAI = EAutoPossessAI::Spawned;
	
	UGameplayStatics::FinishSpawningActor(ShooterSamCharacter, SpawnTransform);

	CanSpawnEnemy = false;
	
	AShooterAI* ShooterAIActor = Cast<AShooterAI>(ShooterSamCharacter->GetController());

	if (ShooterAIActor)
	{
		ShooterAIActor->StartBehaviourTree(Player);
		ShooterAIActor->Spawner = this;
		ShooterAIActors.Add(ShooterAIActor);
	}
	
	return ShooterAIActor;
}

void AEnemySpawner::DestroyEnemy(AShooterAI* Enemy)
{
	for (int i = 0; i < ShooterAIActors.Num(); i++)
	{
		if (ShooterAIActors[i] == Enemy)
		{
			ShooterAIActors.RemoveAt(i);
			Enemy->ThisCharacter->DestroyCharacter();
			return;
		}
	}
}
