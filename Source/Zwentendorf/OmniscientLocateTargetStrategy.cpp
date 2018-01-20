// Fill out your copyright notice in the Description page of Project Settings.

#include "OmniscientLocateTargetStrategy.h"

#include "SoulAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UOmniscientLocateTargetStrategy::UOmniscientLocateTargetStrategy()
	: UStrategy()
{
}

const bool UOmniscientLocateTargetStrategy::ExecuteStrategy(ASoulAIController *soulAIController)
{
	bool bSuccess = false;

	if (soulAIController)
	{
		AActor *targetActor = UGameplayStatics::GetPlayerPawn(soulAIController->GetWorld(), 0);

		if (targetActor)
		{
			UBlackboardComponent* BlackboardComp = soulAIController->GetBlackboardComp();

			AActor *targetActor = UGameplayStatics::GetPlayerPawn(soulAIController->GetWorld(), 0);

			BlackboardComp->SetValueAsObject("TargetActor", targetActor);

			bSuccess = true;
		}
	}

	return bSuccess;
}
