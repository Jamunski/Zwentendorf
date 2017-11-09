// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponAutomator.generated.h"

UCLASS()
class ZWENTENDORF_API AWeaponAutomator : public AActor
{
	GENERATED_BODY()
	
	/* The position component */
	UPROPERTY(Category = Position, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* PositionComponent;

public:	
	AWeaponAutomator();

protected:
	virtual void BeginPlay() override;

private:
	void ShotTimerExpired();

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	UPROPERTY(Category = Module, EditAnywhere)
		AWeaponModule* m_Weapon;

	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AWeaponModule> m_WeaponClass;

public:
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireInterval;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;
};
