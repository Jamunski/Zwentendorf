// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulMovementComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "TimerManager.h"

static const float DODGE_CURVE_LENGTH = 1.0f;

USoulMovementComponent::USoulMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsEvading(false)
	, bCanEvade(true)
	, fEvadeDstanceLastFrame(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);

	bAutoRegisterUpdatedComponent = false;

	ResetMoveState();

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));

	//initialize movement params
	MaxSpeed = 1200.f;
	Acceleration = 4000.f;
	Deceleration = 8000.f;
	TurningBoost = 8.0f;
	bPositionCorrected = false;

	//initialize evade params
	EvadeEnergyCost = 40.0f;
	EvadeDistance = 1000.0f;
	EvadeDuration = 0.5f;
	EvadeRecoveryDuration = 0.25f;
	InvincibilityDuration = 0.25f;
}

void USoulMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PossessedSoul || !UpdatedComponent || !PhysicsMesh)
	{
		return;
	}

	const AController* Controller = PossessedSoul->GetController();
	if (Controller && Controller->IsLocalController() && !bIsEvading)
	{
		FVector InputVector;

		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() || !Controller->IsFollowingAPath() || bUseAccelerationForPaths)
		{
			InputVector = GetInputVector(true);
		}
		else
		{
			// BTMoveTo uses UNavMovementComponent::RequestDirectMove which updates the Velocity member of the movement component with the path direction.
			InputVector = Velocity;
			InputVector.Z = 0.0f;
			InputVector.Normalize();
		}

		FVector PhysicsVelocity = PhysicsMesh->GetPhysicsLinearVelocity();

		//if (!InputVector.IsNearlyZero(1e-6f))
		{
			auto prevGravity = PhysicsVelocity.Z;

			PhysicsVelocity = InputVector * MaxSpeed;

			PhysicsVelocity.Z += prevGravity;
		}

		PhysicsMesh->SetPhysicsLinearVelocity(PhysicsVelocity);

		//JV-TODO: Find out if MovementComponent::SlideAlongSurface will help you here... I think I will need to override and copy most of the function so that it will do the same logic but using SetPhysicsLinearVelocity instead of SafeMoveUpdatedComponent... If this isn't an easy fix, then it may be worth revising solution so that I am not using SetPhysicsLinearVelocity

		Velocity = PhysicsMesh->GetPhysicsLinearVelocity();
		UpdateComponentVelocity();

		//JV-TODO: add some hook to be able to play animations...
	}
}
void USoulMovementComponent::BeginPlay()
{
	if (MyTimeline)
	{
		FOnTimelineFloat callback{};
		callback.BindUFunction(this, FName{ TEXT("EvadeTick") });

		MyTimeline->AddInterpFloat(FloatCurve, callback, FName{ TEXT("MyTimelineAnimation") });
	}
}

FVector USoulMovementComponent::GetInputVector(bool bConsume = false)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	if (bConsume)
	{
		ConsumeInputVector();
	}

	LastInputVector = ControlAcceleration;
	LastInputVector.Normalize();

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

	// If this actor has a parent
	if (Parent)
	{
		ASoul *Soul = Parent->GetOwner()->IsA(ASoul::StaticClass()) ? CastChecked<ASoul>(Parent->GetOwner()) : nullptr;

		// If this actor's parent is a Pawn we can finally return
		return Soul ? Soul : GetParentSoul(Parent);
	}

	return nullptr;
}

void USoulMovementComponent::Evade()
{
	if (!bCanEvade)
	{
		return;
	}
	if (MyTimeline)
	{
		UE_LOG(LogActor, Warning, TEXT("Evade"));

		bool bEvasionSuccess = PossessedSoul->AttemptEnergyConsumption(EvadeEnergyCost);

		if (bEvasionSuccess)
		{
			bIsEvading = true;
			bCanEvade = false;

			float timeMultiplyer = DODGE_CURVE_LENGTH / EvadeDuration;

			MyTimeline->SetPlayRate(timeMultiplyer);
			MyTimeline->PlayFromStart();
		}
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("MyTimeline null"));
	}
}

void USoulMovementComponent::EvadeTick(float val)
{
	float deltaTime = GetWorld()->GetDeltaSeconds();

	float targetDistance = EvadeDistance * val;

	// Distance to travel this tick
	float deltaDistance = targetDistance - fEvadeDstanceLastFrame;

	FVector PhysicsVelocity = PhysicsMesh->GetPhysicsLinearVelocity();

	float finalVelocity = deltaDistance / deltaTime;

	if (!LastInputVector.IsNearlyZero(1e-6f))
	{
		// We want to continue to apply gravity so we capture the zVel to reapply it later
		float prevGravity = PhysicsVelocity.Z;

		PhysicsVelocity = LastInputVector * finalVelocity;

		PhysicsVelocity.Z += prevGravity;
	}

	if (val >= 1.0f - DBL_EPSILON)
	{
		UE_LOG(LogActor, Warning, TEXT("MyTimeline end"));

		bIsEvading = false;
		fEvadeDstanceLastFrame = 0.0f;
		MyTimeline->Stop();

		// Record and consume the input vector
		Velocity = GetInputVector(true);
		PhysicsMesh->SetPhysicsLinearVelocity(Velocity);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_EvadeTimerExpired, this, &USoulMovementComponent::EvadeTimerExpired, EvadeRecoveryDuration);
	}
	else
	{
		PhysicsMesh->SetPhysicsLinearVelocity(PhysicsVelocity);

		fEvadeDstanceLastFrame = targetDistance;
	}
}

void USoulMovementComponent::EvadeTimerExpired()
{
	bCanEvade = true;

	//JV-TODO: indicate to the user that they can now dodge
}

bool USoulMovementComponent::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return bPositionCorrected;
}
