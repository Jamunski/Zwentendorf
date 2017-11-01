// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Soul.generated.h"

UCLASS(abstract)
class ZWENTENDORF_API ASoul : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/* The mesh component */
	UPROPERTY(Category = "Mesh", EditAnywhere)
		class UStaticMeshComponent* MeshComponent;

public:
	ASoul();

	//CALL IN DERIVED
	virtual void InitializeSoul();

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual const float GetHealthPoints() PURE_VIRTUAL(ASoul::GetHealthPoints, return 0.0f ;);

	virtual float ApplyDamage(const float damage) PURE_VIRTUAL(ASoul::GetHealthPoints, return 0.0f ;);

protected:
	void HandleDeath();

	/*
	brief implement to add custom death logic
	*/
	virtual void OnDeath() {};

public:
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
