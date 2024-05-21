// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Aura/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void InitAbilityActorInfo()
	{
	};

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitializePrimaryAttributeGameplayEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitializeSecondaryAttributeGameplayEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitializeVitalAttributeGameplayEffect;
	// [Server]
	void InitializeAttribute() const;
	
	void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GEClass, float Level) const;
	
private:
	void InitializePrimaryAttribute() const;
	void InitializeSecondaryAttribute() const;
	void InitializeVitalAttribute() const;
	
};
