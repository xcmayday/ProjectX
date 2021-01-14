// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectX\Public\ActionRPG\RPGAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "ActionRPG/Item/RPGItem.h"


void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

const FPrimaryAssetType	URPGAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	URPGAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	URPGAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	URPGAssetManager::WeaponItemType = TEXT("Weapon");


URPGAssetManager& URPGAssetManager::Get()
{
	URPGAssetManager* This = Cast<URPGAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		//UE_LOG(LogActionRPG, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<URPGAssetManager>(); // never calls this
	}
}

URPGItem* URPGAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetID, bool bLogWarning /*= true*/)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetID);

	// This does a synchronous load and may hitch
	URPGItem* LoadedItem = Cast<URPGItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		//UE_LOG(LogActionRPG, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetID.ToString());
	}

	return LoadedItem;
}
