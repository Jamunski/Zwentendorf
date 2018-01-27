// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponModule.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponAutomator.generated.h"

class USceneComponent;

UCLASS()
class ZWENTENDORF_API AWeaponAutomator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponAutomator();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float FireInterval;
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
		class USoundBase* FireSound;

protected:
	virtual void BeginPlay() override;

private:
	void ShotTimerExpired();

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	UPROPERTY(Category = Position, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* PositionComponent;
	UPROPERTY(Category = Module, EditAnywhere)
		AWeaponModule* m_Weapon;
	UPROPERTY(Category = Module, EditAnywhere)
		TSubclassOf<AWeaponModule> m_WeaponClass;
};
