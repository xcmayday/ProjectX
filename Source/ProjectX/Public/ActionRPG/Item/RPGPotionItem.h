// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Item/RPGItem.h"
#include "../RPGAssetManager.h"
#include "RPGPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGPotionItem : public URPGItem
{
	GENERATED_BODY()
public:
	URPGPotionItem()
	{
		ItemType = URPGAssetManager::PotionItemType;
	}
	
};
