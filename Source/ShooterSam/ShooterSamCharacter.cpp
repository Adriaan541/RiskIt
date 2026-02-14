// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamCharacter.h"

#include "UI/EnemyHealthBar.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShooterSam.h"
#include "ShooterSamGameMode.h"
#include "ShooterSamPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Gun/Gun.h"
#include "Items/itemDrop.h"
#include "Items/ItemType.h"
#include "Kismet/GameplayStatics.h"

AShooterSamCharacter::AShooterSamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	ItemManager = CreateDefaultSubobject<UItemManager>(TEXT("ItemManager"));
	
	EnemyHealthBarLocation = CreateDefaultSubobject<USceneComponent>(TEXT("EnemyHealthBarLocation"));
	EnemyHealthBarLocation->SetupAttachment(RootComponent);

	EnemyHealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	EnemyHealthBarWidgetComponent->SetupAttachment(EnemyHealthBarLocation);
}

void AShooterSamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &AShooterSamCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::Look);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShooterSamCharacter::AutoShoot);
	}
	else
	{
		UE_LOG(LogShooterSam, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}



void AShooterSamCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AShooterSamCharacter::OnDamageTaken);

	GetMesh()->HideBoneByName("weapon_r", PBO_None);

	BaseMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	ShotIntervalSeconds = BaseShotIntervalSeconds;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (Gun)
	{
		Gun->SetOwner(this);

		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->OwnerController = GetController();
	}

	PlayerController = Cast<AShooterSamPlayerController>(GetController());
	SetExperience(CurrentExperience);

	SetEnemyHealthBar();
	
	Health = MaxHealth;
	
	UpdateHudHealth();
}

void AShooterSamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TimeSinceLastShot < BaseShotIntervalSeconds)
	{
		TimeSinceLastShot += DeltaTime;
	}
	if (TimeSinceEnemyHealthBarVisible < SetEnemyHealthBarInvisibleAfterSeconds)
	{
		TimeSinceEnemyHealthBarVisible += DeltaTime;
	}
	else if (TimeSinceEnemyHealthBarVisible >= SetEnemyHealthBarInvisibleAfterSeconds)
	{
		SetEnemyHealthBarVisible(false);
	}

	UpdateMovementSpeed();

	UpdateFireRate();

	CheckDamageItemUpdate();
	
	RegenHealth(DeltaTime);
}

void AShooterSamCharacter::SetEnemyHealthBar()
{
	if (!PlayerController)
	{
		EnemyHealthBarWidgetComponent->SetWidgetClass(EnemyHealthBarWidgetClass);
		EnemyHealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		EnemyHealthBarWidgetComponent->SetVisibility(false);

		if (UUserWidget* EnemyHealthBarWidgetInstance = EnemyHealthBarWidgetComponent->GetWidget())
		{
			EnemyHealthBarWidget = Cast<UEnemyHealthBar>(EnemyHealthBarWidgetInstance);
		}
	}
}

void AShooterSamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AShooterSamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AShooterSamCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AShooterSamCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AShooterSamCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AShooterSamCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AShooterSamCharacter::AutoShoot()
{
	if (TimeSinceLastShot >= ShotIntervalSeconds)
	{
		Shoot();
		TimeSinceLastShot = 0.0f;
		if (GameMode)
		{
			GameMode->AlertAllEnemies();
		}
	}
}

void AShooterSamCharacter::Shoot()
{
	Gun->PullTrigger();
}

void AShooterSamCharacter::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	SetMaxHealth();
	SetDamage();
}

void AShooterSamCharacter::SetMaxHealth()
{
	MaxHealth = BaseHealth * FMath::Pow(1.2f, Level - 1);
}

void AShooterSamCharacter::CheckDamageItemUpdate()
{
	if (ItemManager && ItemManager->GetItemsMap()[EItemType::Damage] != CurrentDamageItems)
	{
		SetDamage();
	}
}

void AShooterSamCharacter::SetDamage()
{
	if (Gun && ItemManager)
	{
		// Every Damage item (Ammo icon) adds 40% damage
		int AmountOfDamageItem = ItemManager->GetItemsMap()[EItemType::Damage];
		float DamageItemsModifier = 1 + (0.4 * AmountOfDamageItem);
		
		Gun->BulletDamage = Gun->BaseBulletDamage * FMath::Pow(1.2f, Level - 1) * DamageItemsModifier;
	}
}

void AShooterSamCharacter::SetExperience(float ExperienceAmount)
{
	if (GameMode)
	{
		float ExperienceNeededForCalculatedLevel;
		float MaxExperienceCurrentLevel;
		int32 NewLevel = GameMode->GetLevelInfoFromExperience(ExperienceAmount, ExperienceNeededForCalculatedLevel, MaxExperienceCurrentLevel);
		SetLevel(NewLevel);
		if (PlayerController)
		{
			PlayerController->HUDWidget->SetExperienceBar(Level, ExperienceAmount - ExperienceNeededForCalculatedLevel, MaxExperienceCurrentLevel);
		}
	}
}

void AShooterSamCharacter::GainExperience(float ExperienceAmount)
{
	CurrentExperience += ExperienceAmount;
	SetExperience(CurrentExperience);
}

void AShooterSamCharacter::RegenHealth(float DeltaTime)
{
	HealthRegenTimer += DeltaTime;

	if (ItemManager && HealthRegenTimer > 2.0f && Health < MaxHealth)
	{
		if (HealthRegenTimer > 4.0f) HealthRegenTimer = 2.0f;
		else HealthRegenTimer -= 2.0f;

		int HealthKitAmount = ItemManager->GetItemsMap()[EItemType::HealthKit];

		float NewHealth = Health + HealthKitAmount + BaseHealthRegeneration;
		if (NewHealth > MaxHealth) Health = MaxHealth;
		else Health = NewHealth;

		UpdateHudHealth();
	}
}

void AShooterSamCharacter::UpdateMovementSpeed()
{
	if (ItemManager && ItemManager->GetItemsMap()[EItemType::MovementSpeed] != CurrentMovementSpeedItems)
	{
		CurrentMovementSpeedItems = ItemManager->GetItemsMap()[EItemType::MovementSpeed];
		
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed * (1 + CurrentMovementSpeedItems * 0.4f);
	}
}

void AShooterSamCharacter::UpdateFireRate()
{
	if (ItemManager && ItemManager->GetItemsMap()[EItemType::FireRate] != CurrentFireRateItems)
	{
		CurrentFireRateItems = ItemManager->GetItemsMap()[EItemType::FireRate];

		ShotIntervalSeconds = BaseShotIntervalSeconds * FMath::Pow( 0.9f, CurrentFireRateItems);
	}
}

void AShooterSamCharacter::UpdateHudHealth()
{
	if (PlayerController)
	{
		PlayerController->HUDWidget->SetHealthBar(Health, MaxHealth);
	}
	else
	{
		EnemyHealthBarWidget->SetHealthBar(Health, MaxHealth);
	}
}

void AShooterSamCharacter::SetEnemyHealthBarVisible(bool IsVisible)
{
	if (!PlayerController)
	{
		if (!IsVisible)
		{
			TimeSinceEnemyHealthBarVisible = SetEnemyHealthBarInvisibleAfterSeconds;
		}
		else
		{
			TimeSinceEnemyHealthBarVisible = 0.0f;
		}
		EnemyHealthBarWidgetComponent->SetVisibility(IsVisible);
	}
}

void AShooterSamCharacter::DestroyCharacter()
{
	if (Gun)
	{
		Gun->Destroy();
	}
	Destroy();
}

void AShooterSamCharacter::SpawnItemDrop() const
{
	FTransform SpawnTransform(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 90.0f));
	AItemDrop* ItemDrop = GetWorld()->SpawnActorDeferred<AItemDrop>(ItemDropClass, SpawnTransform);

	if (ItemDrop && ItemManager)
	{
		FItem RandomItem = ItemManager->GetRandomItem();

		ItemDrop->ItemType = RandomItem.ItemType;
		ItemDrop->DropMaterial = RandomItem.ItemMaterial;

		UGameplayStatics::FinishSpawningActor(ItemDrop, SpawnTransform);
	}
}

void AShooterSamCharacter::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                         class AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor->GetActorNameOrLabel() == InstigatedBy->GetActorNameOrLabel() | !IsAlive) return;

	bool HealthBarVisibility = true;

	Health -= Damage;

	if (Health <= 0)
	{
		AShooterSamCharacter* InstigatorCharacter = Cast<AShooterSamCharacter>(InstigatedBy->GetPawn());
		HealthBarVisibility = false;
		if (InstigatorCharacter)
		{
			InstigatorCharacter->GainExperience(1.82f * FMath::Sqrt(CurrentExperience) + 20);
		}
		SpawnItemDrop();
		Health = 0;
		IsAlive = false;
		if (GameMode)
		{
			GameMode->CharacterDied(this);
		}
		
		SetActorEnableCollision(false);
		Gun->SetActorEnableCollision(false);

		DetachFromControllerPendingDestroy();
	}
	SetEnemyHealthBarVisible(HealthBarVisibility);
	UpdateHudHealth();
}
