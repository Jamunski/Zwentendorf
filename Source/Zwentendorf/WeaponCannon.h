// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponCannon.generated.h"

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

	virtual void InitializeMesh() override;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

public:
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;
};
