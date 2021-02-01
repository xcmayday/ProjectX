// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectX/Public/ActionRPG/RPGCharacterBase.h"
#include "AbilitySystemGlobals.h"
#include "ProjectX/Public/ActionRPG/Abilities/RPGGameplayAbility.h"
#include "ActionRPG/RPGPlayerStateBase.h"
#include "ActionRPG/RPGPlayerControllerBase.h"


// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	//AttributeSet=CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;

	bAbilitiesInitialized = false;
	

	

}



void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//cancel character in ABS;
	//if (AbilitySystemComponent)
	//{
	//	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	//	AddStartupGameplayAbilities();
	//}

	//Use PS is ABS
	UE_LOG(LogTemp, Warning, TEXT("%s "), *FString(__FUNCTION__));
	
	ARPGPlayerStateBase* PS = GetPlayerState<ARPGPlayerStateBase>();
	if (PS)
	{
		AbilitySystemComponent =Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent()) ;
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		AddStartupGameplayAbilities();

		AttributeSet = PS->GetAttributeSetBase();

		InitializeAttributes();
		ARPGPlayerControllerBase* PC = Cast<ARPGPlayerControllerBase>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
	}

	
}

void ARPGCharacterBase::UnPossessed()
{
	Super::UnPossessed();
	UE_LOG(LogTemp, Warning, TEXT("%s "), *FString(__FUNCTION__));
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ARPGCharacterBase::GetHealth() const
{
	if (!AttributeSet)
	{
		return 1.0f;
	}
	return AttributeSet->GetHealth();
}

float ARPGCharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ARPGCharacterBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float ARPGCharacterBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ARPGCharacterBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

bool ARPGCharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation /*= true*/)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}
	return false;
}

void ARPGCharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 ARPGCharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ARPGCharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		for (TSubclassOf<URPGGameplayAbility>& StartupAbility:GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));

		}
		for (TSubclassOf<UGameplayEffect>& GameplayEffect:PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EfffectContext = AbilitySystemComponent->MakeEffectContext();
			EfffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EfffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),AbilitySystemComponent);
			}
		
		}
		AddSlottedGameplayAbilities();
		bAbilitiesInitialized = true;
	}

}

void ARPGCharacterBase::AddSlottedGameplayAbilities()
{
	TMap<FRPGItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpecs(SlottedAbilitySpecs);
	for (const TPair<FRPGItemSlot,FGameplayAbilitySpec>& SpecPair:SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);
		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
		}
	}

}


void ARPGCharacterBase::FillSlottedAbilitySpecs(TMap<FRPGItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{
	for (const TPair<FRPGItemSlot,TSubclassOf<URPGGameplayAbility>>& DefaultPair:DefaultSlottedAbilities)
	{
		if (DefaultPair.Value.Get())
		{
			SlottedAbilitySpecs.Add(DefaultPair.Key, FGameplayAbilitySpec(DefaultPair.Value, GetCharacterLevel(), INDEX_NONE, this));
			
		}
	}
}

void ARPGCharacterBase::InitializeAttributes()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}

void ARPGCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Warning, TEXT("%s "), *FString(__FUNCTION__));
	ARPGPlayerStateBase* PS = GetPlayerState<ARPGPlayerStateBase>();
	if (PS)
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAttributeSetBase();
		InitializeAttributes();

		ARPGPlayerControllerBase* PC = Cast<ARPGPlayerControllerBase>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
	}
}

void ARPGCharacterBase::OnRep_Controller()
{

}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGCharacterBase, CharacterLevel);
}