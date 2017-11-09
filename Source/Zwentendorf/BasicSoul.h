// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soul.h"
#include "BasicSoul.generated.h"

/**
 * TEMPORARY CLASS: The purpose of this class is to provide a concrete implementation of ASoul to create BP
 */
UCLASS()
class ZWENTENDORF_API ABasicSoul : public ASoul
{
	GENERATED_BODY()
	
public:
	/* The maximum health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
	float MaximumHealthPoints;

	/* The current health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
	float HealthPoints;

public:
	ABasicSoul();

	virtual const float GetHealthPoints() override;

	virtual float ApplyDamage(const float damage) override;

protected:
	void OnDeath() override;

//////////REMOVE ME//////////
	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/* Flag to control firing  */
	uint32 bCanFire : 1;

public: 
	void FireWeapon();

	void ShotTimerExpired();

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;
//////////REMOVE ME//////////
};
