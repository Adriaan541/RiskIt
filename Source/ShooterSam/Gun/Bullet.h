// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterSamCharacter.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTERSAM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* TrailParticles;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	float Damage;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse, const FHitResult& Hit);
};
