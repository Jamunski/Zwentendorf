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
	AMobilityModule();
	virtual void Evade();
};
