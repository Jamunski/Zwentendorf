// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soul.h"
#include "Bob.generated.h"

/**
 * 
 */
UCLASS()
class ZWENTENDORF_API ABob : public ASoul
{
	GENERATED_BODY()
	
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

public:
	/* The maximum health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float MaximumHealthPoints;

	/* The current health points of the robot*/
	UPROPERTY(Category = Health, EditAnywhere, BlueprintReadWrite)
		float HealthPoints;

	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	
public:
	ABob();

	// Begin Soul Interface
	virtual const float GetHealthPoints() override;
	virtual float ApplyDamage(const float damage) override;

	virtual void OnDeath() override;
	// End Soul Interface

public:
	/** Returns Core MeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
