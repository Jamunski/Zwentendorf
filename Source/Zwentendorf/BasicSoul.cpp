// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicSoul.h"

#include "ProjectileCannon.h"

#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"


ABasicSoul::ABasicSoul()
	: ASoul()
{
	//////////REMOVE ME//////////
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Audio/Weapon/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Weapon
	FireRate = 0.25f;
	bCanFire = true;
	//////////REMOVE ME//////////
}

const float ABasicSoul::GetHealthPoints()
{
	return HealthPoints;
}

float ABasicSoul::ApplyDamage(const float damage)
{
	HealthPoints -= damage;

	return HealthPoints;
}

void ABasicSoul::OnDeath()
{
	Destroy();
}

//////////REMOVE ME//////////
void ABasicSoul::FireWeapon()
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		if (GetMeshComponent())
		{
			// Spawn projectile at an offset using cannon dir vector
			const UStaticMeshSocket* fireDirectionSocket = GetMeshComponent()->GetSocketByName("DIR_ProjectileSpawn"); //JV-TODO: Correct this name in the mesh, also account for the radius of the projectile in the mesh file. If you re-export shooting will be broken again...
			FTransform socketTransform;
			fireDirectionSocket->GetSocketTransform(socketTransform, GetMeshComponent());

			const FVector SpawnLocation = socketTransform.GetLocation();
			const FRotator FireRotation = socketTransform.GetRotation().Rotator();

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<AProjectileCannon>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ABasicSoul::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void ABasicSoul::ShotTimerExpired()
{
	bCanFire = true;
}
//////////REMOVE ME//////////
