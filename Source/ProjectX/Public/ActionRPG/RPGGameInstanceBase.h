// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "RPGGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGGameInstanceBase : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	URPGGameInstanceBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FPrimaryAssetType, int32> ItemSlotsPerType;


	
};
