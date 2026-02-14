#pragma once
UENUM(BlueprintType)
enum class EItemType : uint8
{
	LightningZap = 0 UMETA(DisplayName = "LightningZap"),
	HealthKit = 1 UMETA(DisplayName = "HealthKit"),
	Damage = 2 UMETA(DisplayName = "Damage"),
	FireRate = 3 UMETA(DisplayName = "FireRate"),
	MovementSpeed = 4 UMETA(DisplayName = "MovementSpeed"),
};
