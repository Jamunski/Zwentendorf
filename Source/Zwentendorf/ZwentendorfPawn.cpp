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
#include "UObject/ConstructorHelpers.h"

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

	// Core Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshes/Character/Tasis/COR_Core.COR_Core"));
	// Create the mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetStaticMesh(Mesh.Object);

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
}

void AZwentendorfPawn::PostInitializeComponents()
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

			//Mobility
			m_Mobility = World->SpawnActor<AMobilityDefault>(GetActorLocation(), GetActorRotation());

			//Weapons
			m_CannonLeft = World->SpawnActor<ACannon>();
			m_CannonRight = World->SpawnActor<ACannon>();

			if (m_Chassis && m_Mobility && m_CannonLeft && m_CannonRight)
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

					m_CannonLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);
				}

				FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
				{
					UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

					m_CannonRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);
				}

				//Weld parts together...
				m_CannonLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
				m_CannonRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);
				m_Chassis->GetMeshComponent()->WeldTo(m_Mobility->GetMeshComponent(), fnSckChassis);
				MeshComponent->WeldTo(m_Chassis->GetMeshComponent(), fnSckCore);

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

				m_CannonLeft->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponLeft);
				}

				FName fnSckWeaponRight = TEXT("SCK_WeaponRight");
				if (m_Chassis->GetMeshComponent()->DoesSocketExist(fnSckWeaponRight))
				{
				UE_LOG(LogActor, Warning, TEXT("SCK_WeaponLeft Exists"));

				m_CannonRight->AttachToComponent(m_Chassis->GetMeshComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, fnSckWeaponRight);
				}

				//Weld parts together...
				m_Chassis->GetMeshComponent()->WeldTo(MeshComponent, fnSckChassis);
				m_CannonLeft->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponLeft);
				m_CannonRight->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckWeaponRight);
				m_Mobility->GetMeshComponent()->WeldTo(m_Chassis->GetMeshComponent(), fnSckMobility);

				MeshComponent->SetSimulatePhysics(true);
				MeshComponent->SetEnableGravity(true);
				*/
			}
		}
	}
}

void AZwentendorfPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	SetupActionInput(PlayerInputComponent);
	SetupAxisInput(PlayerInputComponent);
	SetupAxisInputKeyboard(PlayerInputComponent);
	SetupAxisInputGamepad(PlayerInputComponent);
}

void AZwentendorfPawn::SetupActionInput(UInputComponent* PlayerInputComponent)
{
	//JV-TODO:
	// set up gameplay weapon bindings
	PlayerInputComponent->BindAction(LeftShoulderBinding, IE_Pressed, this, &AZwentendorfPawn::LeftShoulder);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &AMyCharacter::InteractReleased);
	PlayerInputComponent->BindAction(RightShoulderBinding, IE_Pressed, this, &AZwentendorfPawn::RightShoulder);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &AMyCharacter::InteractReleased);
	PlayerInputComponent->BindAction(LeftTriggerBinding, IE_Pressed, this, &AZwentendorfPawn::LeftTrigger);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &AMyCharacter::InteractReleased);
	PlayerInputComponent->BindAction(RightTriggerBinding, IE_Pressed, this, &AZwentendorfPawn::RightTrigger);
	//PlayerInputComponent->BindAction(InteractBinding, IE_Released, this, &AMyCharacter::InteractReleased);

	//// set up gameplay ability bindings
	PlayerInputComponent->BindAction(InteractBinding, IE_Pressed, this, &AZwentendorfPawn::Interact);
	PlayerInputComponent->BindAction(DodgeBinding, IE_Pressed, this, &AZwentendorfPawn::Dodge);
	PlayerInputComponent->BindAction(AbilityXBinding, IE_Pressed, this, &AZwentendorfPawn::AbilityX);
	PlayerInputComponent->BindAction(AbilityYBinding, IE_Pressed, this, &AZwentendorfPawn::AbilityY);
}

void AZwentendorfPawn::SetupAxisInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(MoveForwardBinding, this, &AZwentendorfPawn::MoveForward);
	PlayerInputComponent->BindAxis(MoveRightBinding, this, &AZwentendorfPawn::MoveRight);

	PlayerInputComponent->BindAxis(AimForwardBinding);
	PlayerInputComponent->BindAxis(AimRightBinding);
}

void AZwentendorfPawn::SetupAxisInputKeyboard(UInputComponent* PlayerInputComponent)
{
}

void AZwentendorfPawn::SetupAxisInputGamepad(UInputComponent* PlayerInputComponent)
{
}

void AZwentendorfPawn::Tick(float DeltaSeconds)
{
	FVector movementVector = ConsumeMovementInputVector();
	m_Mobility->CaclulateMovementInput(DeltaSeconds, movementVector);
	CalculateAimInput();
}

void AZwentendorfPawn::CalculateAimInput()
{
	//// Create fire direction vector
	const float AimForwardValue = GetInputAxisValue(AimForwardBinding);
	const float AimRightValue = GetInputAxisValue(AimRightBinding);
	const FVector AimDirection = FVector(AimForwardValue, AimRightValue, 0.f);

	// If we are pressing aim stick in a direction
	if (AimDirection.SizeSquared() > 0.0f)
	{
		// Rotate the player
		const FRotator NewRotation = AimDirection.Rotation();
		m_Chassis->GetMeshComponent()->SetRelativeRotation(NewRotation); //JV-TODO: Review this.. maybe this function should be part of the chassis? Unless guns will also react to aim input???
	}
}

////////////////////////////ActionBindings////////////////////////////

void AZwentendorfPawn::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		UE_LOG(LogActor, Warning, TEXT("MoveForward-VALID"));

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector and move in that direction 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AZwentendorfPawn::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		UE_LOG(LogActor, Warning, TEXT("MoveRight-VALID"));

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector and move in that direction 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(-Direction, Value);
	}
}

void AZwentendorfPawn::LeftShoulder()
{
	m_CannonLeft->Activate();
}

void AZwentendorfPawn::RightShoulder()
{
	m_CannonRight->Activate();
}
