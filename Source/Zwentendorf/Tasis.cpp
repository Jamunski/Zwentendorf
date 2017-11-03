// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Tasis.h"

//JV-TODO: TEMP PLEASE REMOVE ME...
#include "WeaponCannon.h"
#include "ChassisDefault.h"
#include "MobilityDefault.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATasis::ATasis()
{
	UE_LOG(LogActor, Warning, TEXT("ATasis"));

	// Create the mesh component
	if (MeshComponent)
	{
		RootComponent = MeshComponent;
	}

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when player does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
}

void ATasis::PostInitializeComponents()
{
	UE_LOG(LogActor, Warning, TEXT("PostInitializeComponents"));

	Super::PostInitializeComponents();

	//Creating and attaching modules to sockets
	if (!IsTemplate(RF_Transient))
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FVector location = FVector(0, 0, 10);
			FRotator rotation = FRotator(0.0f);

			//Chassis
			m_Chassis = World->SpawnActor<AChassisDefault>();
			m_Chassis->LinkSoul(this);

			//Mobility
			m_Mobility = World->SpawnActor<AMobilityDefault>(GetActorLocation(), GetActorRotation());
			m_Mobility->LinkSoul(this);

			//Weapons
			m_WeaponLeft = World->SpawnActor<AWeaponCannon>();
			m_WeaponLeft->LinkSoul(this);

			m_WeaponRight = World->SpawnActor<AWeaponCannon>();
			m_WeaponRight->LinkSoul(this);

			if (m_Chassis && m_Mobility && m_WeaponLeft && m_WeaponRight)
			{
				//Chassis
				FName fnSckChassis = TEXT("SCK_Chassis");
				if (m_Mobility->GetMeshComponent()->DoesSocketExist(fnSckChassis))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_Chassis Exists"));

					m_Chassis->AttachToComponent(m_Mobility->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckChassis);
				}

				//Core
				FName fnSckCore = TEXT("SCK_Core");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckCore))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_Core Exists"));

					AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckCore);
				}

				//Weapon
				FName fnSckWeaponLeft = TEXT("SCK_WeaponLeft");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponLeft))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

					m_WeaponLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);
				}

				FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

					m_WeaponRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);
				}

				//Weld parts together...
				m_WeaponLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
				m_WeaponRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);
				m_Chassis->GetMeshComponent()->WeldTo(m_Mobility->GetMeshComponent(), fnSckChassis);
				if (MeshComponent)
				{
					MeshComponent->WeldTo(m_Chassis->GetMeshComponent(), fnSckCore);
				}

				//JV-TODO: More Correct Hierarchy, results not ideal... Maybe this is the way to go, but I should use physicsConstraints instead to work as joints connecting these pieces together? Need to import later assets in order to get the correct sockets...
				/*
				//Mobility
				FName fnSckMobility = TEXT("SCK_Mobility");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckMobility))
				{
				UE_LOG(LogActor, Warning, TEXT("SCK_Mobility Exists"));

				m_Mobility->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckMobility);
				}

				//Core
				FName fnSckChassis = TEXT("SCK_Chassis");
				if (MeshComponent->DoesSocketExist(fnSckChassis))
				{
				UE_LOG(LogActor, Warning, TEXT("SCK_Chassis Exists"));

				m_Chassis->GetMeshComponent()->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckChassis);
				}

				//Weapon
				FName fnSckWeaponLeft = TEXT("SCK_WeaponLeft");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponLeft))
				{
				UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

				m_WeaponLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);
				}

				FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
				{
				UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

				m_WeaponRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);
				}

				//Weld parts together...
				m_Chassis->GetMeshComponent()->WeldTo(MeshComponent, fnSckChassis);
				m_WeaponLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
				m_WeaponRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);
				m_Mobility->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckMobility);

				MeshComponent->SetSimulatePhysics(true);
				MeshComponent->SetEnableGravity(true);
				*/
			}
		}
	}
}

void ATasis::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

const float ATasis::GetHealthPoints()
{
	if (m_Chassis)
	{
		return m_Chassis->HealthPoints;
	}
	return -1;
}

float ATasis::ApplyDamage(const float damage)
{
	if (m_Chassis)
	{
		m_Chassis->HealthPoints -= damage;

		return m_Chassis->HealthPoints;
	}

	return -1;
}

void ATasis::OnDeath()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->RestartLevel();
	//Another way to restart the level...
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ATasis::CaclulateMovementInput(float DeltaSeconds, FVector movementVector)
{
	if (m_Mobility)
	{
		m_Mobility->CaclulateMovementInput(DeltaSeconds, movementVector);
	}
}

void ATasis::CalculateAimInput(float DeltaSeconds, FVector aimVector)
{
	//This logic may belong in the Chassis module, similar to movement in the mobility module

	// If we are pressing aim stick in a direction
	if (aimVector.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = aimVector.Rotation();
		if (m_Chassis)
		{
			m_Chassis->GetMeshComponent()->SetRelativeRotation(NewRotation);
		}
	}
}

////////////////////////////ActionBindings////////////////////////////

void ATasis::LeftShoulder()
{
	if (m_WeaponLeft)
	{
		m_WeaponLeft->Activate();
	}
}

void ATasis::RightShoulder()
{
	if (m_WeaponRight)
	{
		m_WeaponRight->Activate();
	}
}
