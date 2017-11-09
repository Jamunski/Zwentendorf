// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponAutomator.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AWeaponAutomator::AWeaponAutomator()
{
	PositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Position"));
	RootComponent = PositionComponent;

	FireInterval = 0.25f;
}

void AWeaponAutomator::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		if (m_WeaponClass)
		{
			m_Weapon = World->SpawnActor<AWeaponModule>(m_WeaponClass, PositionComponent->GetComponentLocation(), PositionComponent->GetComponentRotation());
			//FVector const* Location=NULL, FRotator const* Rotation=NULL

			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AWeaponAutomator::ShotTimerExpired, FireInterval);
		}
	}

	if (m_Weapon != NULL)
	{
		m_Weapon->GetMeshComponent()->SetSimulatePhysics(false);
		m_Weapon->GetMeshComponent()->SetEnableGravity(false);

		m_Weapon->Activate();
	}
}

void AWeaponAutomator::ShotTimerExpired()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AWeaponAutomator::ShotTimerExpired, FireInterval);
	}

	if (m_Weapon != NULL)
	{
		m_Weapon->Activate();
	}
}

