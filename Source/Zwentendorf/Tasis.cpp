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
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"


ATasis::ATasis()
{
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

			bool bAttachSuccessful = true;

			if (m_Chassis && m_Mobility && m_WeaponLeft && m_WeaponRight)
			{
				//Attach Chassis to Core
				FName fnSckChassis = TEXT("SCK_Chassis");
				if (MeshComponent && MeshComponent->DoesSocketExist(fnSckChassis))
				{
					bAttachSuccessful = m_Chassis->GetRootComponent()->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckChassis);

					Children.Add(m_Chassis);
				}

				if (m_Chassis->GetMeshComponent())
				{
					//Attach Mobility to Chassis
					FName fnSckMobility = TEXT("SCK_Mobility");
					if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckMobility))
					{
						bAttachSuccessful = m_Mobility->GetRootComponent()->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckMobility);

						Children.Add(m_Mobility);
					}

					//Attach Weapons to Chassis
					FName fnSckWeaponLeft = TEXT("SCK_WeaponLeft");
					if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponLeft))
					{
						bAttachSuccessful = m_WeaponLeft->GetRootComponent()->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);

						Children.Add(m_WeaponLeft);
					}

					FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
					if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
					{
						bAttachSuccessful = m_WeaponRight->GetRootComponent()->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);

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
						m_WeaponLeft->GetMeshComponent()->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
						m_WeaponLeft->GetMeshComponent()->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
						m_WeaponLeft->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponRight->GetMeshComponent(), true);

						m_WeaponRight->GetMeshComponent()->IgnoreComponentWhenMoving(MeshComponent, true);
						m_WeaponRight->GetMeshComponent()->IgnoreComponentWhenMoving(m_Chassis->GetMeshComponent(), true);
						m_WeaponRight->GetMeshComponent()->IgnoreComponentWhenMoving(m_Mobility->GetMeshComponent(), true);
						m_WeaponRight->GetMeshComponent()->IgnoreComponentWhenMoving(m_WeaponLeft->GetMeshComponent(), true);
					}
					m_Mobility->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckMobility);
					m_WeaponLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
					m_WeaponRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);

					//Set the movementController's updated component
					SetMCUpdatedComponent(m_Mobility->GetMeshComponent()); //JV-TODO: This needs to be called whenever the mobility component is changed.
				}
			}
		}
	}
}

void ATasis::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATasis::BeginPlay()
{
	UMaterialInstanceDynamic *pCoreColorMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);

	if (pCoreColorMaterialInstance)
	{
		FLinearColor color = { 0.0f, 1.0f, 0.0f, 1.0f };
		pCoreColorMaterialInstance->SetVectorParameterValue(FName("Color"), color);
	}
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
		float remainingHealth = m_Chassis->ApplyDamage(damage);

		UpdateCoreColor();

		return remainingHealth;
	}

	return -1;
}

bool ATasis::AttemptEnergyConsumption(const float amount)
{
	bool bSuccess = false;
	if (m_Chassis)
	{
		auto energyContainer = m_Chassis->DepleteEnergy(amount, bSuccess);
	}

	return bSuccess;
}

FEnergyContainer ATasis::GetEnergyContainer()
{
	FEnergyContainer EnergyContainer;
	if (m_Chassis)
	{
		EnergyContainer = m_Chassis->EnergyContainer;
	}

	return EnergyContainer;
}

void ATasis::UpdateCoreColor()
{
	float healthPercentage = m_Chassis->HealthPoints / m_Chassis->MaximumHealthPoints;

	FLinearColor finalColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	FLinearColor red = { 1.0f, 0.0f, 0.0f, 1.0f };
	FLinearColor green = { 0.0f, 1.0f, 0.0f, 1.0f };
	FLinearColor yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

	float colorLerpPercentage = 0.0f;

	if (healthPercentage > 0.70f)
	{
		float max = 1.0f;
		float min = 0.70f;
		float range = max - min;

		colorLerpPercentage = (healthPercentage - min) / range;

		//Green to Yellow
		finalColor = FLinearColor::LerpUsingHSV(yellow, green, colorLerpPercentage);
	}
	else if (healthPercentage > 0.1f)
	{
		float max = 0.70f;
		float min = 0.2f;
		float range = max - min;

		colorLerpPercentage = (healthPercentage - min) / range;

		//Yellow to Red
		finalColor = FLinearColor::LerpUsingHSV(red, yellow, colorLerpPercentage);
	}
	else
	{
		float max = 0.2f;
		float min = 0.0f;
		float range = max - min;

		colorLerpPercentage = (healthPercentage - min) / range;

		//Red to Black
		finalColor = FLinearColor::LerpUsingHSV(finalColor, red, colorLerpPercentage);
	}

	UMaterialInstanceDynamic *pCoreColorMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);

	if (pCoreColorMaterialInstance)
	{
		pCoreColorMaterialInstance->SetVectorParameterValue(FName("Color"), finalColor);
	}
}

void ATasis::OnDeath()
{
	//JV-TODO: Explode the character and wait for a short period of time before restarting the level. Don't forget to disable input...
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

float ATasis::GetTotalMass()
{
	float totalMass = 0.0f;

	totalMass += m_Mobility->GetMeshComponent()->GetMass();
	totalMass += m_Chassis->GetMeshComponent()->GetMass();
	totalMass += m_WeaponLeft->GetMeshComponent()->GetMass();
	totalMass += m_WeaponRight->GetMeshComponent()->GetMass();

	return totalMass;
}

void ATasis::CalculateAimInput(float DeltaSeconds, FVector aimVector)
{
	// If we are pressing aim stick in a direction
	if (aimVector.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = aimVector.Rotation();
		if (m_Chassis)
		{
			// Collect the mobility previous rotation so that we can reset it whenever we rotate the chassis
			auto mobRot = m_Mobility->GetMeshComponent()->GetComponentRotation();

			m_Chassis->CalculateAimInput(DeltaSeconds, NewRotation);

			m_Mobility->GetMeshComponent()->SetWorldRotation(mobRot, true);
		}
	}
}

////////////////////////////ActionBindings////////////////////////////

void ATasis::ChassisSlot_One_Pressed()
{
	if (m_WeaponLeft)
	{
		m_WeaponLeft->Activate();
	}
}

void ATasis::ChassisSlot_Two_Pressed()
{
	if (m_WeaponRight)
	{
		m_WeaponRight->Activate();
	}
}

void ATasis::Evade_Pressed()
{
	if (m_Mobility)
	{
		m_Mobility->Evade();
	}
}
