// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulMovementComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

USoulMovementComponent::USoulMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);

	MaxSpeed = 1200.f;
	Acceleration = 4000.f;
	Deceleration = 8000.f;
	TurningBoost = 8.0f;
	bPositionCorrected = false;

	bAutoRegisterUpdatedComponent = false;

	ResetMoveState();
}

void USoulMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UE_LOG(LogActor, Warning, TEXT("Begin TickComponent"));

	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PossessedSoul || !UpdatedComponent || !PhysicsMesh)
	{
		return;
	}

	UE_LOG(LogActor, Warning, TEXT("Pass TickComponent"));

	const AController* Controller = PossessedSoul->GetController();
	if (Controller && Controller->IsLocalController())
	{
		FVector InputVector;
		
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() || !Controller->IsFollowingAPath() || bUseAccelerationForPaths)
		{
			InputVector = GetInputVector(DeltaTime);
			UE_LOG(LogActor, Warning, TEXT("%s GetInputVector X: %f, Y: %f, Z: %f"), *GetNameSafe(PossessedSoul), InputVector.X, InputVector.Y, InputVector.Z);
		}
		else
		{

			// BTMoveTo uses UNavMovementComponent::RequestDirectMove which updates the Velocity member of the movement component with the path direction.
			InputVector = Velocity;
			InputVector.Normalize();
			InputVector.Z = 0.0f;
			UE_LOG(LogActor, Warning, TEXT("%s Skip GetInputVector: X: %f, Y: %f, Z: %f"), *GetNameSafe(PossessedSoul), InputVector.X, InputVector.Y, InputVector.Z);
		}

		UE_LOG(LogActor, Warning, TEXT("Pass TickComponent"));

		FVector PhysicsVelocity = PhysicsMesh->GetPhysicsLinearVelocity();

		if (!InputVector.IsNearlyZero(1e-6f))
		{
			auto prevGravity = PhysicsVelocity.Z;

			PhysicsVelocity = InputVector * MaxSpeed;

			PhysicsVelocity.Z += prevGravity;
		}

		PhysicsMesh->SetPhysicsLinearVelocity(PhysicsVelocity);

		//JV-TODO: Find out if MovementComponent::SlideAlongSurface will help you here... I think I will need to override and copy most of the function so that it will do the same logic but using SetPhysicsLinearVelocity instead of SafeMoveUpdatedComponent... If this isn't an easy fix, then it may be worth revising solution so that I am not using SetPhysicsLinearVelocity

		Velocity = PhysicsMesh->GetPhysicsLinearVelocity();
		UpdateComponentVelocity();
	}
};

FVector USoulMovementComponent::GetInputVector(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	ConsumeInputVector();

	return ControlAcceleration;
}

void USoulMovementComponent::SetUpdatedComponent(USceneComponent * NewUpdatedComponent)
{
	if (NewUpdatedComponent)
	{
		PawnOwner = GetParentSoul(NewUpdatedComponent);

		if (!PawnOwner)
		{
			PawnOwner = CastChecked<APawn>(NewUpdatedComponent->GetOwner());
		}

		if (PawnOwner)
		{
			PossessedSoul = CastChecked<ASoul>(PawnOwner);

			UpdatedComponent = NewUpdatedComponent;
			PhysicsMesh = CastChecked<UStaticMeshComponent>(NewUpdatedComponent);
		}
	}
}

ASoul * USoulMovementComponent::GetParentSoul(USceneComponent * NewUpdatedComponent)
{
	USceneComponent *Parent = nullptr;

	if (NewUpdatedComponent && NewUpdatedComponent->GetOwner() && NewUpdatedComponent->GetOwner()->GetRootComponent())
	{
		USceneComponent *thing = NewUpdatedComponent->GetOwner()->GetRootComponent();
		Parent = NewUpdatedComponent->GetOwner()->GetRootComponent()->GetAttachParent();
	}

	//If this actor has a parent
	if (Parent)
	{
		ASoul *Soul = Parent->GetOwner()->IsA(ASoul::StaticClass()) ? CastChecked<ASoul>(Parent->GetOwner()) : nullptr;

		//if this actor's parent is a Pawn we can finally return
		return Soul ? Soul : GetParentSoul(Parent);
	}

	return nullptr;
}

bool USoulMovementComponent::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return bPositionCorrected;
}