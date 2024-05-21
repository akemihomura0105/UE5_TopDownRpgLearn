// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/Interface/EnemyInterface.h"
#include "AuraEnemy.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TArray<USkeletalMeshComponent*> GetHighLightMeshComponentArray();
	virtual void SetActorHighlight(bool bIsHighLight) override;
	virtual int32 GetCombatLevel() override;
	virtual void BeginPlay() override;
	
protected:
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level = 1; 
	
};
