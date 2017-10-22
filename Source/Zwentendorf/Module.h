// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Module.generated.h"

UCLASS(abstract)
class ZWENTENDORF_API AModule : public AActor
{
	GENERATED_BODY()

protected:
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;
	
public:	
	AModule();

protected:
	//Call in derrived constructor
	virtual void InitializeMesh() PURE_VIRTUAL(AModule::InitializeMesh, ;);

public:
	/** Returns MobilityMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
