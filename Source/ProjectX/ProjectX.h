// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

//PROJECTX_API ECLARE_LOG_CATEGORY_EXTERN(LogActionRPG,Log,All);

UENUM(BlueprintType)
enum class EProjectXAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	NormalAttack				UMETA(DisplayName = "NormalAttack"),
	// 2 Cancel
	SpecialAttack				UMETA(DisplayName = "SpecialAttack"),
	// 3 Sprint
	Roll				UMETA(DisplayName = "Roll"),
	// 4 Jump
	ChangeWeapon				UMETA(DisplayName = "ChangeWeapon"),
	// 5 PrimaryFire
	Inventory			UMETA(DisplayName = "Inventory"),
	// 6 SecondaryFire
	Run						UMETA(DisplayName = "Run"),
	// 7 Alternate Fire
	AlternateFire		UMETA(DisplayName = "Alternate Fire"),
	// 8 Reload
	Reload				UMETA(DisplayName = "Reload"),
	// 9 NextWeapon
	NextWeapon			UMETA(DisplayName = "Next Weapon"),
	// 10 PrevWeapon
	PrevWeapon			UMETA(DisplayName = "Previous Weapon"),
	// 11 Interact
	Interact			UMETA(DisplayName = "Interact")
};