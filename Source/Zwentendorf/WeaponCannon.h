// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponCannon.generated.h"

class AProjectile;

UCLASS()
class ZWENTENDORF_API AWeaponCannon : public AWeaponModule
{
	GENERATED_BODY()

public:
	AWeaponCannon();

	void Activate() override;

	void ShotTimerExpired();

protected:
	virtual void BeginPlay() override;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

public:
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;
	UPROPERTY(Category = Projectile, EditAnywhere)
		TSubclassOf<AProjectile> ProjectileClass;
};
