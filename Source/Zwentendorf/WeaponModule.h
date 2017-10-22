// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Module.h"
#include "WeaponModule.generated.h"

/**
*
*/
UCLASS(abstract)
class ZWENTENDORF_API AWeaponModule : public AModule
{
	GENERATED_BODY()

public:
	AWeaponModule();

	virtual void Activate() PURE_VIRTUAL(AWeaponModule::Activate, ;);
};
