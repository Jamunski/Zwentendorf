// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWaitingForInputFunctor.h"

#include "SoulPlayerController.h"
#include "ZwentendorfGameMode.h"

ULobbyWaitingForInputFunctor::ULobbyWaitingForInputFunctor()
{
	Delegate_WaitingForInput.BindLambda([](ASoulPlayerController* thisPlayerController)
	{
		check(thisPlayerController);

		UWorld* world = thisPlayerController->GetWorld();
		if (world)
		{
			AZwentendorfGameMode* gameMode = Cast<AZwentendorfGameMode>(world->GetAuthGameMode());

			ASoulPlayerController* playerToSwitchInput = Cast<ASoulPlayerController>(gameMode->GetFirstAvailablePlayerController());

			if (playerToSwitchInput)
			{
				// Get the player which should receive this player's controller index
				int32 controllerID = playerToSwitchInput->GetLocalPlayer()->GetControllerId();

				thisPlayerController->GetLocalPlayer()->SetControllerId(controllerID);

				ASoul* soulToPossess = Cast<ASoul>(gameMode->GetNextUnpossessedPawn());

				if (soulToPossess)
				{
					UE_LOG(LogInput, Log, TEXT("valid soulToPossess"));

					playerToSwitchInput->UnbindGameplayInput(false);
					playerToSwitchInput->BindGameplayInput();

					playerToSwitchInput->Possess(soulToPossess);
				}
			}
		}
	});
}

