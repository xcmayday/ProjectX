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

	void CreateHUD();

	class URPGUserWidget* GetGSHUD();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASShooter|UI")
		TSubclassOf<class URPGUserWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "GASShooter|UI")
		class URPGUserWidget* UIHUDWidget;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;


	UFUNCTION(Exec)
		void Kill();

	UFUNCTION(Server, Reliable)
	void ServerKill();
	bool ServerKill_Validate();
	
};
