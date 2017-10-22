// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module.h"
#include "MobilityModule.generated.h"

/**
 * 
 */
UCLASS(abstract)
class ZWENTENDORF_API AMobilityModule : public AModule
{
	GENERATED_BODY()
	
public:
	/* The speed our pawn moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

public:
	AMobilityModule();

	virtual void CaclulateMovementInput(float DeltaSeconds, FVector movementVector) PURE_VIRTUAL(AWeaponModule::Activate, ;);
};
