// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileMovementComponent> Projectile;
};
