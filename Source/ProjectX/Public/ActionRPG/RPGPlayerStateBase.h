// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "RPGPlayerStateBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRPGOnGameplayAttributeValueChangedDelegate, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);
/**
 * 
 */
UCLASS()
class PROJECTX_API ARPGPlayerStateBase : public APlayerState,public IAbilitySystemInterface
{

	GENERATED_BODY()
public:
	ARPGPlayerStateBase();

	virtual void ClientInitialize(class AController* C)override;
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class URPGAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "RPG|RPGPlayerState")
		bool IsAlive() const;


	UFUNCTION(BlueprintCallable, Category = "RPG|RPGPlayerState|Attributes")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPG|RPGPlayerState|Attributes")
		float GetMaxHealth() const;


protected:
	virtual void BeginPlay()override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

protected:
	UPROPERTY()
		class URPGAbilitySystemComponent* RPGAbilitySystemComponent;

	UPROPERTY()
		class URPGAttributeSet* RPGAttributeSetBase;

	// Attribute changed delegate handles
	FDelegateHandle HealthChangedDelegateHandle;

};
