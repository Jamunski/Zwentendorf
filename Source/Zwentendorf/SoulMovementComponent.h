// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

#include "Soul.h"

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SoulMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class ZWENTENDORF_API USoulMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

	ASoul *PossessedSoul;

	UStaticMeshComponent *PhysicsMesh;

	//Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//End UActorComponent Interface

	//Begin UMovementComponent Interface
	virtual float GetMaxSpeed() const override { return MaxSpeed; }

public:
	/** Overridden to allow registration of any USceneComponent* */
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	/*Recursive function which travels up the hierarchy until it finds a Pawn. Returns nullptr if it fails to find a Pawn*/
	virtual ASoul *GetParentSoul(USceneComponent* NewUpdatedComponent);

protected:
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
	//End UMovementComponent Interface

public:
	UPROPERTY(Category = "Strafe", EditAnywhere, BlueprintReadWrite)
		bool bStrafing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float Deceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement, meta = (ClampMin = "0", UIMin = "0"))
		float TurningBoost;

protected:

	virtual FVector GetInputVector(float DeltaTime);

	UPROPERTY(Transient)
		uint32 bPositionCorrected : 1;
};
