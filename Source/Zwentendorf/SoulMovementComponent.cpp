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
	//UE_LOG(LogActor, Warning, TEXT("Begin TickComponent"));

	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PossessedSoul || !UpdatedComponent || !PhysicsMesh)
	{
		return;
	}

	//UE_LOG(LogActor, Warning, TEXT("Pass TickComponent"));

	const AController* Controller = PossessedSoul->GetController();
	if (Controller && Controller->IsLocalController())
	{
		FVector InputVector;

		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() || !Controller->IsFollowingAPath() || bUseAccelerationForPaths)
		{
			InputVector = GetInputVector(DeltaTime);
		}

		if (!InputVector.IsNearlyZero(1e-6f))
		{
			PhysicsMesh->AddImpulse(InputVector * 10 * Acceleration); //Need to set simulate physics for this to work...
			FVector PhysicsVelocity = PhysicsMesh->GetPhysicsLinearVelocity();
			if (PhysicsVelocity.Size() > MaxSpeed)
			{
				PhysicsMesh->SetPhysicsLinearVelocity(InputVector.GetClampedToSize(0.0f, MaxSpeed));
			}
		}

		// Finalize
		//JV-TODO: Get the velocity and set the member. Velocity = physicsVel;
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