// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobilityDefault.generated.h"

UCLASS()
class ZWENTENDORF_API AMobilityDefault : public AActor
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

public:
	AMobilityDefault();

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

	void CaclulateMovementInput(float DeltaSeconds, FVector movementVector);

protected:
	virtual void BeginPlay() override;

public:
	/** Returns MobilityMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};

