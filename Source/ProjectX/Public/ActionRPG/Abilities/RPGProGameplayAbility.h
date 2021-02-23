// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../ProjectX.h"
#include "RPGProGameplayAbility.generated.h"

/**
 * 
 */
class USkeletalMeshComponent;


USTRUCT()
struct PROJECTX_API FAbilityMeshMontage
{
	GENERATED_BODY()
public:
	UPROPERTY()
		class USkeletalMeshComponent* Mesh;
	UPROPERTY()
		class UAnimMontage* Montage;
	FAbilityMeshMontage():Mesh(nullptr), Montage(nullptr)
	{

	}
	FAbilityMeshMontage(class USkeletalMeshComponent* InMesh, class UAnimMontage* InMontage)
		:Mesh(InMesh),Montage(InMontage)
	{
	}
};


UCLASS()
class PROJECTX_API URPGProGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URPGProGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EProjectXAbilityInputID AbilityInputID = EProjectXAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool bActivateAbilityOnGranted;
	
};
