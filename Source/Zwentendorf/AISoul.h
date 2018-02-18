// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soul.h"
#include "AISoul.generated.h"

/**
 * 
 */
class AProjectile;

UCLASS()
class ZWENTENDORF_API AAISoul : public ASoul
{
	GENERATED_BODY()
private:
	uint32 bCanFire : 1;

public:

	virtual const float GetHealthPoints() override;
	virtual float ApplyDamage(const float damage) override;
	virtual void OnDeath() override;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	AAISoul();

	UFUNCTION(BlueprintCallable, Category = "Soul")
	void Attack();
	
	void ShotTimerExpired();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Health;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireRate;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = Projectile, EditAnywhere)
		TSubclassOf<AProjectile> ProjectileClass;
	
};
