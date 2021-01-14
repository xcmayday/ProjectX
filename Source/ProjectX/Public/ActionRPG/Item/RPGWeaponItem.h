// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Item/RPGItem.h"
#include "../RPGAssetManager.h"
#include "RPGWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGWeaponItem : public URPGItem
{
	GENERATED_BODY()
public:
	URPGWeaponItem()
	{
		ItemType = URPGAssetManager::WeaponItemType;
	}
	
};
