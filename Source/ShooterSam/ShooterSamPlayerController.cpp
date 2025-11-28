// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterSamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ItemDrop.h"
#include "Blueprint/UserWidget.h"
#include "ShooterSam.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/GameplayTask_SpawnActor.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AShooterSamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogShooterSam, Error, TEXT("Could not spawn mobile controls widget."));

		}
	}

	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToPlayerScreen();
		HUDWidget->ItemWidgetClass = ItemBoxWidgetClass;

		HUDWidget->ItemImages = HUDItemImages;
		TArray<FItemValues> ItemValues;
		ItemValues.Add({FString::Printf(TEXT("Ammo")), 1});
		ItemValues.Add({FString::Printf(TEXT("Ammo2")), 200});
		
		HUDWidget->UpdateItems(ItemValues);
		ItemValues[0] = {FString::Printf(TEXT("Ammo")), 2};
		HUDWidget->UpdateItems(ItemValues);
	}

	FTransform SpawnTransform;
	AItemDrop* ItemDrop = GetWorld()->SpawnActorDeferred<AItemDrop>(ItemDropClass, SpawnTransform);

	// AItemDrop* ItemDrop = GetWorld()->SpawnActor<AItemDrop>(ItemDropClass);
	if (ItemDrop)
	{
		ItemDrop->DropMaterial = ItemDropMaterial;
		UE_LOG(LogShooterSam, Display, TEXT("ItemDrop created"));

		UGameplayStatics::FinishSpawningActor(ItemDrop, SpawnTransform);
	}
}

void AShooterSamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}
