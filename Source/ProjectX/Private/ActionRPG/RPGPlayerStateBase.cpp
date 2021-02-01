// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRPG/RPGPlayerStateBase.h"
#include "ActionRPG/Abilities/RPGAbilitySystemComponent.h"
#include "ActionRPG/Abilities/RPGAttributeSet.h"

/*if (World->WorldType == EWorldType::PIE)
{
	switch (World->GetNetMode())
	{
	case NM_Client:
		Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		Prefix = FString::Printf(TEXT("Server: "));
		break;
	case NM_Standalone:
		break;
	}
}*/



ARPGPlayerStateBase::ARPGPlayerStateBase()
{
	// Create ability system component, and set it to be explicitly replicated
	RPGAbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("RPGAbilitySystemComponent"));
	RPGAbilitySystemComponent->SetIsReplicated(true);

	RPGAttributeSetBase = CreateDefaultSubobject<URPGAttributeSet>(TEXT("RPGAttributeSet"));

	NetUpdateFrequency = 66.0f;
}

void ARPGPlayerStateBase::ClientInitialize(class AController* C)
{
	Super::ClientInitialize(C);
	UE_LOG(LogTemp, Warning, TEXT("%s "), *FString(__FUNCTION__));
}

class UAbilitySystemComponent* ARPGPlayerStateBase::GetAbilitySystemComponent() const
{
	return RPGAbilitySystemComponent;
}

class URPGAttributeSet* ARPGPlayerStateBase::GetAttributeSetBase() const
{
	return RPGAttributeSetBase;
}

bool ARPGPlayerStateBase::IsAlive() const
{
	return RPGAttributeSetBase->GetHealth() > 0;
	
}

float ARPGPlayerStateBase::GetHealth() const
{
	return RPGAttributeSetBase->GetHealth();
}

float ARPGPlayerStateBase::GetMaxHealth() const
{
	return RPGAttributeSetBase->GetMaxHealth();
}

void ARPGPlayerStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGPlayerStateBase::HealthChanged(const FOnAttributeChangeData& Data)
{

}
