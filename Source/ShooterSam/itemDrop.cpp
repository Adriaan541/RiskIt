// Fill out your copyright notice in the Description page of Project Settings.


#include "itemDrop.h"

#include "ShooterSamCharacter.h"

// Sets default values
AItemDrop::AItemDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(SceneRoot);

	IconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IconMesh"));
	IconMesh->SetupAttachment(CapsuleCollision);
}

// Called when the game starts or when spawned
void AItemDrop::BeginPlay()
{
	Super::BeginPlay();
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemDrop::OnOverlapBegin);

	IconMesh->SetMaterial(0, DropMaterial);
}

// Called every frame
void AItemDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rotation = FRotator(0, 100.0f * DeltaTime, 0);
	AddActorLocalRotation(Rotation);
}

void AItemDrop::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AShooterSamCharacter* OtherCharacter = Cast<AShooterSamCharacter>(OtherActor);
		if (OtherCharacter)
		{
			UItemManager* CharacterItemManager = OtherCharacter->ItemManager;
			if (CharacterItemManager)
			{
				CharacterItemManager->AddItem("Ammo");
				OtherCharacter->UpdateHUDItems();
			}
		}
	}
}

