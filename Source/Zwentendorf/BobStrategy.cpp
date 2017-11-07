// Fill out your copyright notice in the Description page of Project Settings.

#include "BobStrategy.h"


UBobStrategy::UBobStrategy()
	: UStrategy()
{
}

const bool UBobStrategy::ExecuteStrategy(ASoulAIController *soulAIController)
{
	bool bSuccess = false;

	UE_LOG(LogActor, Warning, TEXT("UBobStrategy::ExecuteStrategy"));

	return bSuccess;
}

