// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraProjectile.h"

#include <array>

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SetRootComponent(CollisionSphere);
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.SetAllChannels(ECR_Ignore);
	ResponseContainer.SetResponse(ECC_WorldDynamic, ECR_Overlap);
	ResponseContainer.SetResponse(ECC_WorldStatic, ECR_Overlap);
	ResponseContainer.SetResponse(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannels(ResponseContainer);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	Projectile->InitialSpeed = 500;
	Projectile->MaxSpeed = 500;
	Projectile->ProjectileGravityScale = 0.f;
}
