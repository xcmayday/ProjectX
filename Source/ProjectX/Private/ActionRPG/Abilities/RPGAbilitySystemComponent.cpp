// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectX/Public/ActionRPG/Abilities/RPGAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URPGAbilitySystemComponent::URPGAbilitySystemComponent()
{

}

void URPGAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec:AbilitiesToActivate)
	{
		if (Spec)
		{

			TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();
			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(Cast<URPGGameplayAbility>(ActiveAbility));
			}
		}
		
	}

}

int32 URPGAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	return 1;
}

URPGAbilitySystemComponent* URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<URPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
