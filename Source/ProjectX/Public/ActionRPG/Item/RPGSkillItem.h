// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPG/Item/RPGItem.h"
#include "../RPGAssetManager.h"
#include "RPGSkillItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGSkillItem : public URPGItem
{
	GENERATED_BODY()
public:
	URPGSkillItem()
	{
		ItemType = URPGAssetManager::SkillItemType;
	}
	
};
