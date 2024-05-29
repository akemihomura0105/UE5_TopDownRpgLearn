// Fill out your copyright notice in the Description page of Project Settings.


#include "AureProjectileSpell.h"

#include "Aura/Actor/AuraProjectile.h"
#include "Aura/Character/AuraCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UAureProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthority(&ActivationInfo)) return;
	FTransform ProjectileTransform;
	auto Projectile = GetWorld()->SpawnActorDeferred<AActor>(
		ProjectileClass,
		ProjectileTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	auto Character = Cast<AAuraCharacterBase>(GetAvatarActorFromActorInfo());
	ProjectileTransform.SetLocation(Character->GetCombatProjectileGenerateLocation());
	ProjectileTransform.SetRotation(Character->GetTransform().GetRotation());

	Projectile->FinishSpawning(ProjectileTransform);
}
