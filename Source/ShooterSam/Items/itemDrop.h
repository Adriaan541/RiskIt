// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "itemDrop.generated.h"

enum class EItemType : uint8;

UCLASS()
class SHOOTERSAM_API AItemDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemDrop();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollision;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* IconMesh;

	EItemType ItemType;

	UMaterial* DropMaterial;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

};
