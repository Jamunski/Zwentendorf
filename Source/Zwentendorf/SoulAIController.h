// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Soul.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoulAIController.generated.h"

UCLASS()
class ZWENTENDORF_API ASoulAIController : public AAIController
{
	GENERATED_BODY()

protected: 
	UPROPERTY(EditDefaultsOnly, Category = "Soul")
	ASoul *PossessedSoul;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	AActor *TargetActor;

	/*Behavior tree comp ref*/
	UBehaviorTreeComponent* BehaviorComp;

	/*----------Blackboard----------*/

	/*Blackboard comp ref*/
	UBlackboardComponent* BlackboardComp;

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LocationToGoKey;

	/*----------Blackboard----------*/

	/*I use the AActor class as the Template class so I can use the GetAllActorsOfClass function.
	However, in this collection I'm going to store all the Available ABotTargetPoint instances.*/
	TArray<AActor*> BotTargetPoints;

	/*Posses is executed when the character we want to control is spawned.
	Inside this function, we initialize the blackboard and start the behavior tree*/
	virtual void Possess(APawn* Pawn) override;

public:
	ASoulAIController();

	virtual void SetTarget(AActor *Target);

	/*----------Strategies----------*/
	void ExecuteStrategy(EStrategyType strategy);

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }
};
