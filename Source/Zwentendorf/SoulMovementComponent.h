// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

#include "Soul.h"

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SoulMovementComponent.generated.h"

class UTimelineComponent;

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class ZWENTENDORF_API USoulMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

public:
	/** Overridden to allow registration of any USceneComponent* */
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	/*Recursive function which travels up the hierarchy until it finds a Pawn. Returns nullptr if it fails to find a Pawn*/
	virtual ASoul *GetParentSoul(USceneComponent* NewUpdatedComponent);

	/*How this actor evades*/
	virtual void Evade();

	UFUNCTION()
	void EvadeTick(float val);

	// Movement Params
	UPROPERTY(Category = SoulMovement, EditAnywhere, BlueprintReadWrite)
		float MaxSpeed;
	UPROPERTY(Category = SoulMovement, EditAnywhere, BlueprintReadWrite)
		float Acceleration;
	UPROPERTY(Category = SoulMovement, EditAnywhere, BlueprintReadWrite)
		float Deceleration;
	UPROPERTY(Category = SoulMovement, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float TurningBoost;
	// Evade Params
	UPROPERTY(Category = Evade, EditAnywhere, BlueprintReadWrite)
		float EvadeEnergyCost;
	UPROPERTY(Category = Evade, EditAnywhere, BlueprintReadWrite)
		float EvadeDistance;
	UPROPERTY(Category = Evade, EditAnywhere, BlueprintReadWrite)
		float EvadeDuration;
	UPROPERTY(Category = Evade, EditAnywhere, BlueprintReadWrite)
		float EvadeRecoveryDuration;
	UPROPERTY(Category = Evade, EditAnywhere, BlueprintReadWrite)
		float InvincibilityDuration;

protected:
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;

	/** Handle for efficient management of EvadeTimerExpired timer */
	FTimerHandle TimerHandle_EvadeTimerExpired;

	virtual FVector GetInputVector(bool bConsume);

	UPROPERTY(Transient)
		uint32 bPositionCorrected : 1;
	UPROPERTY(Category = "Evade", EditAnywhere)
		UCurveFloat* FloatCurve;

	FVector LastInputVector;
	UTimelineComponent* MyTimeline;
	bool bIsEvading;
	bool bCanEvade;
	float fEvadeDstanceLastFrame;

private:
	//Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;
	//End UActorComponent Interface

	virtual float GetMaxSpeed() const override { return MaxSpeed; }

	void EvadeTimerExpired();

	ASoul *PossessedSoul;
	UStaticMeshComponent *PhysicsMesh;
};
