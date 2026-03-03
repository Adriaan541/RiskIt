// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Gun/Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(SceneRoot);

	MuzzleFlashParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashParticleSystem->SetupAttachment(SkeletalMesh);

	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
	BulletSpawnPoint->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	MuzzleFlashParticleSystem->Deactivate();

	BulletDamage = BaseBulletDamage;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	MuzzleFlashParticleSystem->Activate(true);
	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	}

	if (OwnerController)
	{
		FVector ViewpointLocation;
		FRotator ViewpointRotation;
		OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);

		FVector BulletEndpoint = ViewpointLocation + ViewpointRotation.Vector() * MaxRange;

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(HitResult, ViewpointLocation, BulletEndpoint, ECC_GameTraceChannel2,
		                                     Params);
		FVector Direction;
		if (HitResult.bBlockingHit)
		{
			Direction = HitResult.Location - BulletSpawnPoint->GetComponentLocation();
		}
		else
		{
			Direction = BulletEndpoint - BulletSpawnPoint->GetComponentLocation();
		}
		
		Direction.Normalize();
		FTransform SpawnTransform(Direction.Rotation(), BulletSpawnPoint->GetComponentLocation());
		ABullet* Bullet = GetWorld()->SpawnActorDeferred<ABullet>(BulletClass, SpawnTransform);
		
		if (Bullet)
		{
			Bullet->Damage = BulletDamage;
			Bullet->SetOwner(this);
			UGameplayStatics::FinishSpawningActor(Bullet, SpawnTransform);
		}
	}
}
