// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"

#include "Aura/AbilitySystem/AuraAbilitySystem.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystem>("AbilitySystem");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

TArray<USkeletalMeshComponent*> AAuraEnemy::GetHighLightMeshComponentArray()
{
	return {GetMesh(), Weapon};
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AAuraEnemy::SetActorHighlight(bool bIsHighLight)
{
	if (bHighlighted == bIsHighLight)
	{
		return;
	}
	bHighlighted = bIsHighLight;
	TArray<USkeletalMeshComponent*> HighlightMeshComponentArray = {GetMesh(), Weapon};
	for (auto HightlightMesh : GetHighLightMeshComponentArray())
	{
		if (!HightlightMesh)
		{
			continue;
		}
		if (bIsHighLight)
		{
			HightlightMesh->SetRenderCustomDepth(true);
			constexpr uint32 HighLightStencilValue = 250;
			HightlightMesh->SetCustomDepthStencilValue(HighLightStencilValue);
		}
		else
		{
			HightlightMesh->SetRenderCustomDepth(false);
		}
	}
}
