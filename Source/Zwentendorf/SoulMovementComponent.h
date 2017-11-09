// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

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

	//Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//End UActorComponent Interface

	//Begin UMovementComponent Interface
	virtual float GetMaxSpeed() const override { return MaxSpeed; }

protected:
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
	//End UMovementComponent Interface

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement)
		float Deceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoulMovement, meta = (ClampMin = "0", UIMin = "0"))
		float TurningBoost;

protected:

	virtual void ApplyControlInputToVelocity(float DeltaTime);

	virtual bool LimitWorldBounds();

	UPROPERTY(Transient)
		uint32 bPositionCorrected : 1;
};
