// Fill out your copyright notice in the Description page of Project Settings.

#include "BobStrategy.h"

#include "SoulAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UBobStrategy::UBobStrategy()
	: UStrategy()
{
}

const bool UBobStrategy::ExecuteStrategy(ASoulAIController *soulAIController)
{
	bool bSuccess = false;

	if (soulAIController)
	{
		UBlackboardComponent* BlackboardComp = soulAIController->GetBlackboardComp();

		AActor *targetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (targetActor)
		{
			//This breaks when player is upside down...
			soulAIController->MoveToActor(targetActor, 500.0f);

			bSuccess = true;
		}
	}

	return bSuccess;
}
