// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API URPGUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
* Attribute setters
*/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealthRegenRate(float HealthRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxStamina(float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentStamina(float CurrentStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetStaminaRegenRate(float StaminaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxMana(float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentMana(float CurrentMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetManaRegenRate(float ManaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxShield(float MaxShield);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentShield(float CurrentShield);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetShieldPercentage(float ShieldPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetShieldRegenRate(float ShieldRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetExperience(int32 Experience);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHeroLevel(int32 HeroLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetGold(int32 Gold);
	
};
