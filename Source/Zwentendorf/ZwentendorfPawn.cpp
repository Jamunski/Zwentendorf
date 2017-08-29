// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfPawn.h"
#include "ZwentendorfProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

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
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
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

void AZwentendorfPawn::FireShot()
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		const FRotator FireRotation = RootComponent->GetComponentRotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile
			World->SpawnActor<AZwentendorfProjectile>(SpawnLocation, FireRotation);
		}

		bCanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AZwentendorfPawn::ShotTimerExpired, FireRate);

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		bCanFire = false;
	}
}

void AZwentendorfPawn::ShotTimerExpired()
{
	bCanFire = true;
}

////////////////////////////ActionBindings////////////////////////////

void AZwentendorfPawn::RightShoulder()
{
	FireShot();
}

