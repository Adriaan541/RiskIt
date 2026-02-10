// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ShooterSamCharacter.h"

#include "LightningZap.generated.h"

class UNiagaraComponent;

UCLASS()
class SHOOTERSAM_API ALightningZap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningZap();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TargetLocation;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BaseLocation;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* LightningZapSystem;

	UPROPERTY(EditAnywhere)
	float DamageExecutionTime = 0.1f;

	UPROPERTY(EditAnywhere)
	float ChainProcRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float OriginalZapSize = 560.0f;

	FProcChainData ProcChainData;
	
	int AmountOfItemsOnInstigator;

	float Timer;

	bool HasProcced = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitLightningZap(FProcChainData SetProcChainData, int AmountOfItem);
};
