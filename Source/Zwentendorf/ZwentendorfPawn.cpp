// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

const FName AZwentendorfPawn::MoveForwardBinding("MoveForward");
const FName AZwentendorfPawn::MoveRightBinding("MoveRight");
const FName AZwentendorfPawn::AimForwardBinding("AimForward");
const FName AZwentendorfPawn::AimRightBinding("AimRight");

const FName AZwentendorfPawn::LeftShoulderBinding("Weapon-LeftShoulder");
const FName AZwentendorfPawn::RightShoulderBinding("Weapon-RightShoulder");
const FName AZwentendorfPawn::LeftTriggerBinding("Weapon-LeftTrigger");
const FName AZwentendorfPawn::RightTriggerBinding("Weapon-RightTrigger");
const FName AZwentendorfPawn::InteractBinding("Interact");
const FName AZwentendorfPawn::DodgeBinding("Dodge");
const FName AZwentendorfPawn::AbilityXBinding("Ability-X");
const FName AZwentendorfPawn::AbilityYBinding("Ability-Y");

AZwentendorfPawn::AZwentendorfPawn()
{
	UE_LOG(LogActor, Warning, TEXT("AZwentendorfPawn"));

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(30.0f); //JV-TODO: changing this removes my collision problems...
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
}

void AZwentendorfPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Creating and attaching modules to sockets
	if (!IsTemplate(RF_Transient))
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FVector location = FVector(0, 0, 0);
			FRotator rotation = FRotator(0.0f);

			//Mobility
			m_Mobility = World->SpawnActor<AMobilityDefault>(location, rotation);

			m_Mobility->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

			if (m_Mobility != nullptr)
			{
				//Chassis
				m_Chassis = World->SpawnActor<AChassisDefault>();

				FName fnSckChassis = TEXT("SCK_Chassis");
				if (m_Mobility->GetMeshComponent()->DoesSocketExist(fnSckChassis))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_Chassis Exists"));

					m_Chassis->AttachToComponent(m_Mobility->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckChassis);
				}

				//Weapon
				if (m_Chassis != nullptr)
				{
					m_CannonLeft = World->SpawnActor<ACannon>();

					FName fnSckWeaponLeft = TEXT("SCK_WeaponLeft");
					if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponLeft))
					{
						UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

						m_CannonLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);
					}

					m_CannonRight = World->SpawnActor<ACannon>();

					FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
					if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
					{
						UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

						m_CannonRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);
					}

					if (m_CannonLeft != nullptr && m_CannonRight != nullptr)
					{
						//JV-TODO: Review this - Ignore other meshes attached to this... Maybe there is a better way to do this???
						m_Chassis->GetMeshComponent()->IgnoreActorWhenMoving(m_CannonLeft, true);
						m_Chassis->GetMeshComponent()->IgnoreActorWhenMoving(m_Mobility, true);

						m_CannonLeft->GetMeshComponent()->IgnoreActorWhenMoving(m_Chassis, true);
						m_CannonLeft->GetMeshComponent()->IgnoreActorWhenMoving(m_Mobility, true);

						m_CannonRight->GetMeshComponent()->IgnoreActorWhenMoving(m_Chassis, true);
						m_CannonRight->GetMeshComponent()->IgnoreActorWhenMoving(m_Mobility, true);

						m_Mobility->GetMeshComponent()->IgnoreActorWhenMoving(m_CannonLeft, true);
						m_Mobility->GetMeshComponent()->IgnoreActorWhenMoving(m_CannonRight, true);
						m_Mobility->GetMeshComponent()->IgnoreActorWhenMoving(m_Chassis, true);

						//JV-TODO: Review this - Welding meshes together...Better?
						//m_CannonLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
						//m_CannonRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);
						//m_Chassis->GetMeshComponent()->WeldTo(m_Mobility->GetMeshComponent(), fnSckChassis);
					}
				}
			}
		}
	}
}

void AZwentendorfPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay axis bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(AimForwardBinding);
	PlayerInputComponent->BindAxis(AimRightBinding);

	// set up gameplay weapon bindings
	PlayerInputComponent->BindAction(LeftShoulderBinding, IE_Pressed, this, &AZwentendorfPawn::LeftShoulder);
	PlayerInputComponent->BindAction(RightShoulderBinding, IE_Pressed, this, &AZwentendorfPawn::RightShoulder);
	PlayerInputComponent->BindAction(LeftTriggerBinding, IE_Pressed, this, &AZwentendorfPawn::LeftTrigger);
	PlayerInputComponent->BindAction(RightTriggerBinding, IE_Pressed, this, &AZwentendorfPawn::RightTrigger);

	// set up gameplay ability bindings
	PlayerInputComponent->BindAction(InteractBinding, IE_Pressed, this, &AZwentendorfPawn::Interact);
	PlayerInputComponent->BindAction(DodgeBinding, IE_Pressed, this, &AZwentendorfPawn::Dodge);
	PlayerInputComponent->BindAction(AbilityXBinding, IE_Pressed, this, &AZwentendorfPawn::AbilityX);
	PlayerInputComponent->BindAction(AbilityYBinding, IE_Pressed, this, &AZwentendorfPawn::AbilityY);
}

void AZwentendorfPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		RootComponent->K2_AddRelativeLocation(Movement, true, Hit, false);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->AddRelativeLocation(Deflection, true);
		}
	}

	// Create fire direction vector
	const float AimForwardValue = GetInputAxisValue(AimForwardBinding);
	const float AimRightValue = GetInputAxisValue(AimRightBinding);
	const FVector AimDirection = FVector(AimForwardValue, AimRightValue, 0.f);

	Aim(AimDirection);
}

void AZwentendorfPawn::Aim(FVector AimDirection)
{
	// If we are pressing aim stick in a direction
	if (AimDirection.SizeSquared() > 0.0f)
	{
		// Rotate the player
		const FRotator NewRotation = AimDirection.Rotation();
		RootComponent->SetRelativeRotation(NewRotation);
	}
}

////////////////////////////ActionBindings////////////////////////////

void AZwentendorfPawn::LeftShoulder()
{
	m_CannonLeft->Activate();
}

void AZwentendorfPawn::RightShoulder()
{
	m_CannonRight->Activate();
}

