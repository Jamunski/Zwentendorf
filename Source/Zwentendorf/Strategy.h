// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Strategy.generated.h"

class ASoulAIController;

UCLASS(abstract, BlueprintType, Blueprintable)
class ZWENTENDORF_API UStrategy : public UObject
{
	GENERATED_BODY()
	
public:
	UStrategy();

	virtual const bool ExecuteStrategy(ASoulAIController *soulAIController) PURE_VIRTUAL(UStrategy::ExecuteStrategy, return false;);
};
