// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Item/RPGItem.h"
#include "../RPGAssetManager.h"
#include "RPGTokenItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGTokenItem : public URPGItem
{
	GENERATED_BODY()
public:
	URPGTokenItem()
	{
		ItemType = URPGAssetManager::TokenItemType;
	}
	
};
