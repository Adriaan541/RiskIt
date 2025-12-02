// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/Bullet.h"

#include "Gun.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;

	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticles"));
	TrailParticles->SetupAttachment(BulletMesh);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	AGun* OwnerGun = Cast<AGun>(MyOwner);
	AActor* MyCharacter = MyOwner->GetOwner();
	
	if (MyOwner && OwnerGun && MyCharacter)
	{
		if (OtherActor && (OtherActor != MyOwner) && (OtherActor != MyCharacter) && (OtherActor != this))
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerGun->OwnerController, this, UDamageType::StaticClass());
			if (HitParticles)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation(), GetActorRotation());
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			}
		}
	}
	Destroy();
}