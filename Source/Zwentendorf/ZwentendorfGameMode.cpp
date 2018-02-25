// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZwentendorfGameMode.h"
#include "Soul.h"
#include "SoulPlayerController.h"

#include "Zwentendorf.h"

#include <Runtime/Engine/Classes/Engine/LocalPlayer.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AZwentendorfGameMode::AZwentendorfGameMode()
{
	NumberOfPlayers = 8;
	MaxPawnsToPossess = 4;

	PlayerControllerClass = ASoulPlayerController::StaticClass();
}

void AZwentendorfGameMode::BeginPlay()
{
	TArray<AActor*>  FoundControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerControllerClass, FoundControllers);

	for (auto controller : FoundControllers)
	{
		auto soulController = Cast<ASoulPlayerController>(controller);
		PlayerControllers.Add(soulController);
	}

	// Begin
	Super::BeginPlay();

	// get how many players have already been created
	int createdPlayers = GetNumPlayers();

	// if the number of created players is less than the number of players we want for this game mode, create more...
	while (createdPlayers < NumberOfPlayers)
	{
		auto playerController = UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
		PlayerControllers.Add(playerController);
		createdPlayers++;
	}

	ASoul * soulType = SoulToPossess.GetDefaultObject();

	if (soulType)
	{
		TArray<AActor*>  FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), soulType->StaticClass(), FoundActors);

		for (auto actor : FoundActors)
		{
			auto soul = Cast<ASoul>(actor);
			PossessableSouls.Add(soul);
		}
	}
}

APawn * AZwentendorfGameMode::GetNextUnpossessedPawn()
{
	ensure(PossessableSouls.Num() <= MaxPawnsToPossess);

	for (int i = 0; i < PossessableSouls.Num(); i++)
	{
		auto soul = PossessableSouls[i];

		if (soul && !soul->IsPossessed())
		{
			return soul;
		}
		else
		{
			continue;
		}
	}

	UE_LOG(LogSoul, Warning, TEXT("no available pawn to possess from %d possessable pawns"), PossessableSouls.Num());
	return nullptr;
}

APlayerController * AZwentendorfGameMode::GetFirstAvailablePlayerController()
{
	for (int i = 0; i < PlayerControllers.Num(); i++)
	{
		auto controller = PlayerControllers[i];

		if (controller && !Cast<ASoulPlayerController>(controller)->GetPossessedSoul())
		{
			return PlayerControllers[i];
		}
	}

	UE_LOG(LogSoul, Warning, TEXT("no available controller from %d controllers"), PlayerControllers.Num());
	return nullptr;
}
