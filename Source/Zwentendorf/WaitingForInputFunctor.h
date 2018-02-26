// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaitingForInputFunctor.generated.h"

class ASoulPlayerController;

/**
 *
 */
UCLASS(abstract, BlueprintType, Blueprintable)
class ZWENTENDORF_API UWaitingForInputFunctor : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FWaitingForInputDelegate, ASoulPlayerController*);

	virtual bool operator()(ASoulPlayerController* thisPlayerController);

protected:
	FWaitingForInputDelegate Delegate_WaitingForInput;
};
