// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Tasis.h"

//JV-TODO: TEMP PLEASE REMOVE ME...
#include "WeaponCannon.h"
#include "ChassisDefault.h"
#include "MobilityDefault.h"
#include "SoulMovementComponent.h"

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

void ATasis::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogActor, Warning, TEXT("ATasis::OnHit"));
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
			if (m_ChassisClass)
			{
				m_Chassis = World->SpawnActor<AChassisModule>(m_ChassisClass);
				m_Chassis->LinkSoul(this);
			}

			//Mobility 
			if (m_MobilityClass)
			{
				m_Mobility = World->SpawnActor<AMobilityModule>(m_MobilityClass, GetActorLocation(), GetActorRotation());
				m_Mobility->LinkSoul(this);
			}

			//Weapons
			if (m_WeaponLeftClass)
			{
				m_WeaponLeft = World->SpawnActor<AWeaponModule>(m_WeaponLeftClass);
				m_WeaponLeft->LinkSoul(this);
			}

			if (m_WeaponRightClass)
			{
				m_WeaponRight = World->SpawnActor<AWeaponModule>(m_WeaponRightClass);
				m_WeaponRight->LinkSoul(this);
			}

			if (m_Chassis && m_Mobility && m_WeaponLeft && m_WeaponRight)
			{
				//Attach Chassis to Core
				FName fnSckChassis = TEXT("SCK_Chassis");
				if (MeshComponent && MeshComponent->DoesSocketExist(fnSckChassis))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_Chassis Exists"));

					m_Chassis->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckChassis);

					Children.Add(m_Chassis);
				}

				//Attach Mobility to Chassis
				FName fnSckMobility = TEXT("SCK_Mobility");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckMobility))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_Mobility Exists"));

					m_Mobility->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckMobility);

					Children.Add(m_Mobility);
				}

				//Attach Weapons to Chassis
				FName fnSckWeaponLeft = TEXT("SCK_WeaponLeft");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponLeft))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

					m_WeaponLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);

					Children.Add(m_WeaponLeft);
				}

				FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_WeaponRight Exists"));

					m_WeaponRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);

					Children.Add(m_WeaponRight);
				}

				//Weld parts together...
				if (MeshComponent)
				{
					m_Chassis->GetMeshComponent()->WeldTo(MeshComponent, fnSckChassis);

					//Ignore other meshes so that this doesn't collide into itself when moving...
					MeshComponent->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
					MeshComponent->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
					MeshComponent->IgnoreComponentWhenMoving(m_WeaponLeft->GetMeshComponent(), true);
					MeshComponent->IgnoreComponentWhenMoving(m_WeaponRight->GetMeshComponent(), true);

					m_Chassis->GetMeshComponent()->IgnoreComponentWhenMoving(MeshComponent, true);
					m_Chassis->GetMeshComponent()->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
					m_Chassis->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponLeft->GetMeshComponent(), true);
					m_Chassis->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponRight->GetMeshComponent(), true);

					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(MeshComponent, true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponLeft->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponRight->GetMeshComponent(), true);

					m_WeaponLeft->GetMeshComponent()->IgnoreComponentWhenMoving(MeshComponent, true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponRight->GetMeshComponent(), true);

					m_WeaponRight->GetMeshComponent()->IgnoreComponentWhenMoving(MeshComponent, true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
					m_Mobility->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponLeft->GetMeshComponent(), true);
				}
				m_Mobility->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckMobility);
				m_WeaponLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
				m_WeaponRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);

				//Set the movementController's updated component
				SetMCUpdatedComponent(m_Mobility->GetMeshComponent()); //JV-TODO: MC will only tick if the updated component shares the same parent as it.
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

void ATasis::SetMCUpdatedComponent(USceneComponent * NewUpdatedComponent)
{
	if (m_Mobility)
	{
		USoulMovementComponent *MC = m_Mobility->FindComponentByClass<USoulMovementComponent>();
		if (MC && RootComponent)
		{
			MC->SetUpdatedComponent(NewUpdatedComponent);
		}
	}
}

void ATasis::CaclulateMovementInput(float DeltaSeconds, FVector movementVector)
{
	if (m_Mobility)
	{
		//m_Mobility->CaclulateMovementInput(DeltaSeconds, movementVector);
		UPawnMovementComponent* movementComp = GetMovementComponent();

		if (movementComp)
		{

		}
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
