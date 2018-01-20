// Fill out your copyright notice in the Description page of Project Settings.

#include "MobilityDefault.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

AMobilityDefault::AMobilityDefault()
	: AMobilityModule()
{
	MoveSpeed = 1000.0f;
}

void AMobilityDefault::BeginPlay()
{
	Super::BeginPlay();
}

void AMobilityDefault::CaclulateMovementInput(float DeltaSeconds, FVector movementVector)
{
	//JV-TODO: This is dead code, maybe this should hook into the movement controller somehow. The movement controller will need some way to tell this object to animate...
	// Calculate  movement
	//const FVector Movement = movementVector.ClampMaxSize(1.0f) * MoveSpeed * DeltaSeconds;

	//// If non-zero size, move this actor
	//if (Movement.SizeSquared() > 0.0f)
	//{
	//	FHitResult Hit(1.f);
	//	MeshComponent->MoveComponent(Movement, GetMeshComponent()->RelativeRotation, false);

	//	if (Hit.IsValidBlockingHit())
	//	{
	//		const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
	//		const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
	//	}
	//}
}
