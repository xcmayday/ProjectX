// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectX\ProjectX.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/ScriptInterface.h"
#include "ProjectX/Public/ActionRPG/Abilities/RPGAbilitySystemComponent.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Abilities/RPGAttributeSet.h"
#include "RPGType.h"
#include "RPGCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ARPGCharacterBase*, Character);
class URPGGameplayAbility;

UCLASS()
class PROJECTX_API ARPGCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();
	virtual void PossessedBy(AController* NewController)override;
	virtual void UnPossessed()override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
		virtual float GetMana() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
		virtual float GetMaxMana() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
		virtual float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilies")
		void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGGameplayAbility*>& ActiveAbilities);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
		virtual int32 GetCharacterLevel() const;


	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	/** Adds slotted item abilities if needed */
	void AddSlottedGameplayAbilities();

	/** Fills in with ability specs, based on defaults and inventory */
	void FillSlottedAbilitySpecs(TMap<FRPGItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	// Client only
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<URPGGameplayAbility>> GameplayAbilities;
	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;
	/** Map of item slot to gameplay ability class, these are bound before any abilities added by the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
		TMap<FRPGItemSlot, TSubclassOf<URPGGameplayAbility>> DefaultSlottedAbilities;
	UPROPERTY()
	URPGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	URPGAttributeSet* AttributeSet;
	/** If true we have initialized our abilities */
	UPROPERTY()
	int32 bAbilitiesInitialized;

	/** Map of slot to ability granted by that slot. I may refactor this later */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FRPGItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;
	// Default attributes for a character for initializing on spawn/respawn.
// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPG|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	FDelegateHandle InventoryUpdateHandle;
	FDelegateHandle InventoryLoadedHandle;


};
