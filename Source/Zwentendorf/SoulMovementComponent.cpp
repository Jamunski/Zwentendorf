// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"

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

	if (PawnOwner && PawnOwner->IsA(ASoul::StaticClass()))
	{
		PossessedSoul = Cast<ASoul>(PawnOwner);
	}
	else
	{
		//Error msg...
	}

	ResetMoveState();
}

void USoulMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	//DoThing();

	const AController* Controller = PawnOwner->GetController();
	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() || !Controller->IsFollowingAPath() || bUseAccelerationForPaths)
		{
			//JV-TODO: I don't like this way of doing things, Velocity should always represent the world vel, this should return a different vector which is used to apply a change in velocity...
			ApplyControlInputToVelocity(DeltaTime);
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		LimitWorldBounds();
		bPositionCorrected = false;

		// Move actor
		const FVector Gravity = { 0.0f, 0.0f, 9.81f };
		//Velocity += Gravity;

		FVector Delta = Velocity * DeltaTime;

		if (!Delta.IsNearlyZero(1e-6f))
		{
			UE_LOG(LogActor, Warning, TEXT("Moving"), Velocity.X, Velocity.Y, Velocity.Z);

			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = bStrafing ? PawnOwner->GetActorRotation().Quaternion() : Velocity.ToOrientationQuat(); 

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Delta);
				// Try to slide the remaining distance along the surface.
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			if (!bPositionCorrected)
			{
				const FVector NewLocation = UpdatedComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / DeltaTime);
			}
		}

		// Finalize
		UpdateComponentVelocity();
	}
};

bool USoulMovementComponent::LimitWorldBounds()
{
	AWorldSettings* WorldSettings = PawnOwner ? PawnOwner->GetWorldSettings() : NULL;
	if (!WorldSettings || !WorldSettings->bEnableWorldBoundsChecks || !UpdatedComponent)
	{
		return false;
	}

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	if (CurrentLocation.Z < WorldSettings->KillZ)
	{
		Velocity.Z = FMath::Min(GetMaxSpeed(), WorldSettings->KillZ - CurrentLocation.Z + 2.0f);
		return true;
	}

	return false;
}

void USoulMovementComponent::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	Velocity += ControlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

void USoulMovementComponent::SetUpdatedComponent(USceneComponent * NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	//JV-TODO: It might be a good idea to take another look at PawnMovementComponent implementation of this function, maybe the logic there is a good fallback in case this function returns null?
	if (NewUpdatedComponent)
	{
		PawnOwner = GetParentPawn(NewUpdatedComponent);

		if (!PawnOwner)
		{
			PawnOwner = CastChecked<APawn>(NewUpdatedComponent->GetOwner());

			UpdatedComponent = NewUpdatedComponent;
		}
	}
	
}

APawn * USoulMovementComponent::GetParentPawn(USceneComponent * NewUpdatedComponent)
{
	//JV-TODO: The Actor which has the SoulMovementComponenet attached to it is the mobility module. This module is at the top of the actor hierarchy and this function will not work in this case. I think I need to change the hierarchy so that it is correct. Soul <- Chassis <- Mobility, LeftWea, RightWea, Mod1, Mod2

	USceneComponent *Parent = nullptr;

	if (NewUpdatedComponent && NewUpdatedComponent->GetOwner() && NewUpdatedComponent->GetOwner()->GetRootComponent())
	{
		USceneComponent *thing = NewUpdatedComponent->GetOwner()->GetRootComponent();
		Parent = NewUpdatedComponent->GetOwner()->GetRootComponent()->GetAttachParent();
	}

	//If this actor has a parent
	if (Parent)
	{
		APawn *Pawn = CastChecked<APawn>(Parent->GetOwner());

		//if this actor's parent is a Pawn we can finally return
		return Pawn ? Pawn : GetParentPawn(Parent);
	}

	return nullptr;
}

bool USoulMovementComponent::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return bPositionCorrected;
}