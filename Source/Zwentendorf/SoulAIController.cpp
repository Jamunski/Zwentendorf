// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulAIController.h"

#include "BotTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

ASoulAIController::ASoulAIController()
{
	//Initialize BehaviorTreeComponent, BlackboardComponent and the corresponding key

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	LocationToGoKey = "LocationToGo";
}

void ASoulAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	//Get the possessed Character and check if it's my own AI Character
	PossessedSoul = Cast<ASoul>(Pawn);

	if (PossessedSoul)
	{
		//If the blackboard is valid initialize the blackboard for the corresponding behavior tree
		if (PossessedSoul->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(PossessedSoul->BehaviorTree->BlackboardAsset));
		}

		/*Populate the array of available bot target points
		The following function needs a TArray of AActors, that's why I declared the
		BotTargetPoints as such. When I will need to get an exact point and compare it,
		I will cast it to the corresponding class (ABotTargetPoint)*/
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABotTargetPoint::StaticClass(), BotTargetPoints);

		//Start the behavior tree which corresponds to the specific character
		BehaviorComp->StartTree(*PossessedSoul->BehaviorTree);
	}
}

void ASoulAIController::SetTarget(AActor *Target)
{
	TargetActor = Target;
}

void ASoulAIController::ExecuteStrategy(EStrategyType strategy)
{
	if (PossessedSoul)
	{
		bool bSuccess = PossessedSoul->ExecuteStrategy(strategy);
	}
}
