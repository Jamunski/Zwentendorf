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

	UBehaviorTreeComponent* BehaviorComp;

	/*----------Blackboard----------*/

	/*Blackboard comp ref*/
	UBlackboardComponent* BlackboardComp;

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LocationToGoKey;

	/*----------Blackboard----------*/

	TArray<AActor*> BotTargetPoints;

	virtual void Possess(APawn* Pawn) override;

public:
	ASoulAIController();

	virtual void SetTarget(AActor *Target);

	virtual void FireWeapon();

	/*----------Strategies----------*/
	void ExecuteStrategy(EStrategyType strategy);

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }
};
