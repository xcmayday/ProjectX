// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRPG/RPGPlayerControllerBase.h"
#include "ActionRPG/RPGPlayerStateBase.h"
#include "ActionRPG/RPGUserWidget.h"
#include <ActionRPG/Abilities/RPGAttributeSet.h>

void ARPGPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGPlayerControllerBase::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ARPGPlayerStateBase* PS = GetPlayerState<ARPGPlayerStateBase>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<URPGUserWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth() / PS->GetMaxHealth());
	//UIHUDWidget->SetCurrentMana(PS->GetMana());
	//UIHUDWidget->SetMaxMana(PS->GetMaxMana());
	//UIHUDWidget->SetManaPercentage(PS->GetMana() / PS->GetMaxMana());
	//UIHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
	//UIHUDWidget->SetManaRegenRate(PS->GetManaRegenRate());
	//UIHUDWidget->SetCurrentStamina(PS->GetStamina());
	//UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
	//UIHUDWidget->SetStaminaPercentage(PS->GetStamina() / PS->GetMaxStamina());
	//UIHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
	//UIHUDWidget->SetCurrentShield(PS->GetShield());
	//UIHUDWidget->SetMaxShield(PS->GetMaxShield());
	//UIHUDWidget->SetShieldRegenRate(PS->GetShieldRegenRate());
	//UIHUDWidget->SetShieldPercentage(PS->GetShield() / PS->GetMaxShield());
	//UIHUDWidget->SetExperience(PS->GetXP());
	//UIHUDWidget->SetGold(PS->GetGold());
	//UIHUDWidget->SetHeroLevel(PS->GetCharacterLevel());
}

class URPGUserWidget* ARPGPlayerControllerBase::GetGSHUD()
{
	return UIHUDWidget;
}

void ARPGPlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARPGPlayerControllerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	CreateHUD();
}

void ARPGPlayerControllerBase::Kill()
{
	ServerKill();
}

void ARPGPlayerControllerBase::ServerKill_Implementation()
{
	ARPGPlayerStateBase* PS = GetPlayerState<ARPGPlayerStateBase>();
	if (PS)
	{
		PS->GetAttributeSetBase()->SetHealth(0.0f);
		
	}
}

bool ARPGPlayerControllerBase::ServerKill_Validate()
{
	return true;
}
