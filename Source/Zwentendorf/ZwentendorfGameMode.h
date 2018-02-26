// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZwentendorfGameMode.generated.h"

class ASoul;
class UWaitingForInputFunctor;

UCLASS(MinimalAPI)
class AZwentendorfGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZwentendorfGameMode();

	virtual void BeginPlay() override;
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation) override;

	virtual void SetWaitingForInputFunctorClass(TSubclassOf<UWaitingForInputFunctor> functor);
	virtual UWaitingForInputFunctor* CreateWaitingForInputFunctor();

	virtual APawn* GetNextUnpossessedPawn();

	virtual APlayerController* GetFirstAvailablePlayerController();

protected:
	UPROPERTY(Category = Players, EditDefaultsOnly)
		TSubclassOf<ASoul> SoulToPossess;

	UPROPERTY(Category = Players, VisibleAnywhere)
		TArray<ASoul*> PossessableSouls;

	UPROPERTY(Category = Players, VisibleAnywhere)
		TArray<APlayerController*> PlayerControllers;

	UPROPERTY(Category = Players, EditDefaultsOnly)
		int32 NumberOfPlayers;

	UPROPERTY(Category = Players, EditDefaultsOnly)
		int32 MaxPawnsToPossess;

	UPROPERTY(Category = SoulInput, EditDefaultsOnly)
		TSubclassOf<UWaitingForInputFunctor> WaitingForInputFunctorClass;
};
