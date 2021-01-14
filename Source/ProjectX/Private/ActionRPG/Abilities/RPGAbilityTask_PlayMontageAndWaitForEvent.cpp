// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectX/Public/ActionRPG/Abilities/RPGAbilityTask_PlayMontageAndWaitForEvent.h"
#include <ActionRPG/Abilities/RPGAbilitySystemComponent.h>
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"




URPGAbilityTask_PlayMontageAndWaitForEvent::URPGAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
	
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability==nullptr)
	{
		return;
	}
	bool bPlayedMontage = false;
	URPGAbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();
	if (RPGAbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance!=nullptr)
		{
			//�󶨻ص�
			EventHandle = RPGAbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags,
				FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &URPGAbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent));
			if (RPGAbilitySystemComponent->PlayMontage(Ability,Ability->GetCurrentActivationInfo(),MontageToPlay,Rate,StartSection)>0.f)
			{
				if (ShouldBroadcastAbilityTaskDelegates()==false)
				{
					return;
				}
				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &URPGAbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &URPGAbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &URPGAbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character&&(Character->GetLocalRole()==ROLE_Authority||
					(Character->GetLocalRole()==ROLE_AutonomousProxy&&Ability->GetNetExecutionPolicy()==EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}
				bPlayedMontage = true;
			}

		}
		else
		{
			ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
	}
	if (!bPlayedMontage)
	{
		ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{

			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	SetWaitingOnAvatar();
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);
	OnAbilityCancelled();
	Super::ExternalCancel();
}

FString URPGAbilityTask_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bInOwnerFinished&&bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	URPGAbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();
	if (RPGAbilitySystemComponent)
	{
		RPGAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}
	Super::OnDestroy(bInOwnerFinished);
}

URPGAbilityTask_PlayMontageAndWaitForEvent* URPGAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay,
	FGameplayTagContainer EventTags, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	URPGAbilityTask_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<URPGAbilityTask_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags= EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;

	return MyObj;
}

bool URPGAbilityTask_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance==nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent&&Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility()==Ability&&AbilitySystemComponent->GetCurrentMontage()==MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}
			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;

}

URPGAbilitySystemComponent* URPGAbilityTask_PlayMontageAndWaitForEvent::GetTargetASC()
{
	return Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability&&Ability->GetCurrentMontage()==MontageToPlay)
	{
		if (Montage==MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character&&(Character->GetLocalRole()==ROLE_Authority||
				(Character->GetLocalRole()==ROLE_AutonomousProxy&&Ability->GetNetExecutionPolicy()==EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.0f);
			}

		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void URPGAbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	EndTask();


}

void URPGAbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;
		EventReceived.Broadcast(EventTag, TempData);
	}
}
