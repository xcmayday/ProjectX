// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API ARPGPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerControllerBase(){}
	virtual void BeginPlay()override;
	
};
