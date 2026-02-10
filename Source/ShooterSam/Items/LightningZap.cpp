// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LightningZap.h"

#include "NiagaraComponent.h"
#include "ShooterSamGameMode.h"
#include "Gun/Gun.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightningZap::ALightningZap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	TargetLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TargetLocation"));
	TargetLocation->SetupAttachment(SceneRoot);

	BaseLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BaseLocation"));
	BaseLocation->SetupAttachment(SceneRoot);

	LightningZapSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningZapSystem"));
	LightningZapSystem->SetupAttachment(BaseLocation);
}

// Called when the game starts or when spawned
void ALightningZap::BeginPlay()
{
	Super::BeginPlay();

	FVector Direction = TargetLocation->GetComponentLocation() - BaseLocation->GetComponentLocation();
	float EffectScale = Direction.Length() / OriginalZapSize;
	FString VarName = "Scale";
	LightningZapSystem->SetVariableFloat(FName(*VarName), EffectScale);
	UE_LOG(LogTemp, Display, TEXT("Direction.Length(): %f"), Direction.Length());
	UE_LOG(LogTemp, Display, TEXT("Scale: %f"), EffectScale);

	Direction.Normalize();
	BaseLocation->SetWorldRotation(Direction.Rotation());
}

// Called every frame
void ALightningZap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (Timer < DamageExecutionTime)
	{
		const float AmountOfItemsDamageModifier = AmountOfItemsOnInstigator > 1 ? (AmountOfItemsOnInstigator - 1) * 1.25f : 1;
		const float Damage = DeltaTime * (1/DamageExecutionTime) * ProcChainData.BaseDamage * AmountOfItemsDamageModifier;
		// const float Damage = DeltaTime * (1 / DamageExecutionTime) * ProcChainData.BaseDamage;
		if (!ProcChainData.IsNull())
		{
			UGameplayStatics::ApplyDamage(ProcChainData.GetHitActor(), Damage,
			                              ProcChainData.Instigator->OwnerController,
			                              this, UDamageType::StaticClass());
		}
	}
	else
	{
		Destroy();
	}

	if (!ProcChainData.IsNull() && !HasProcced && Timer > DamageExecutionTime / 2)
	{
		if (!ProcChainData.OwnerShooterSamCharacter->PlayerController) return;
		
		AShooterSamCharacter* NewTarget = ProcChainData.OwnerShooterSamCharacter->GameMode->GetNextEnemyForProcChain(
			ProcChainData);
		if (NewTarget != nullptr)
		{
			FProcChainData NewProcData = FProcChainData(ProcChainData);
			NewProcData.HitCharacters.Add(NewTarget);
			ProcChainData.OwnerShooterSamCharacter->ItemManager->OnHitProcChain(&NewProcData);
		}
		HasProcced = true;
	}
}

void ALightningZap::InitLightningZap(FProcChainData SetProcChainData, int AmountOfItem)
{
	// if (SetProcChainData.GetPreviousHit())
	// 	BaseLocation->SetWorldLocation(SetProcChainData.GetPreviousHit()->GetActorLocation());
	if (AShooterSamCharacter* PreviousHit = SetProcChainData.GetPreviousHit())
		BaseLocation->SetWorldLocation(PreviousHit->GetActorLocation());
	else
		BaseLocation->SetWorldLocation(SetProcChainData.OwnerShooterSamCharacter->GetActorLocation());

	TargetLocation->SetWorldLocation(SetProcChainData.GetHitActor()->GetActorLocation());
	SetOwner(SetProcChainData.OwnerShooterSamCharacter);

	ProcChainData = SetProcChainData;
	AmountOfItemsOnInstigator = AmountOfItem;
}
