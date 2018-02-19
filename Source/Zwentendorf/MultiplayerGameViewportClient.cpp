// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerGameViewportClient.h"

#include "Engine/GameEngine.h"
#include "Engine/LocalPlayer.h"

#include <Runtime/EngineSettings/Classes/GameMapsSettings.h>

UMultiplayerGameViewportClient::UMultiplayerGameViewportClient(const FObjectInitializer &ObjectInitializer)
{
	MaxSplitscreenPlayers = 8;
}

void UMultiplayerGameViewportClient::UpdateActiveSplitscreenType()
{
	//Copy pasta from engine source. Only done to comment out ensure(NumPlayers == 4);

	ESplitScreenType::Type SplitType = ESplitScreenType::None;
	const int32 NumPlayers = GEngine->GetNumGamePlayers(GetWorld());
	const UGameMapsSettings* Settings = GetDefault<UGameMapsSettings>();

	if (Settings->bUseSplitscreen && !bDisableSplitScreenOverrideHack)
	{
		switch (NumPlayers)
		{
		case 0:
		case 1:
			SplitType = ESplitScreenType::None;
			break;

		case 2:
			switch (Settings->TwoPlayerSplitscreenLayout)
			{
			case ETwoPlayerSplitScreenType::Horizontal:
				SplitType = ESplitScreenType::TwoPlayer_Horizontal;
				break;

			case ETwoPlayerSplitScreenType::Vertical:
				SplitType = ESplitScreenType::TwoPlayer_Vertical;
				break;

			default:
				check(0);
			}
			break;

		case 3:
			switch (Settings->ThreePlayerSplitscreenLayout)
			{
			case EThreePlayerSplitScreenType::FavorTop:
				SplitType = ESplitScreenType::ThreePlayer_FavorTop;
				break;

			case EThreePlayerSplitScreenType::FavorBottom:
				SplitType = ESplitScreenType::ThreePlayer_FavorBottom;
				break;

			default:
				check(0);
			}
			break;

		default:
			//ensure(NumPlayers == 4);
			SplitType = ESplitScreenType::FourPlayer;
			break;
		}
	}
	else
	{
		SplitType = ESplitScreenType::None;
	}

	ActiveSplitscreenType = SplitType;
}

void UMultiplayerGameViewportClient::SetDisableSplitscreenOverrideHack(const bool bDisabled)
{
	bDisableSplitScreenOverrideHack = bDisabled;
	LayoutPlayers();
}

void UMultiplayerGameViewportClient::LayoutPlayersHack()
{
	UpdateActiveSplitscreenType();
	const ESplitScreenType::Type SplitType = GetCurrentSplitscreenConfiguration();

	// Initialize the players
	const TArray<ULocalPlayer*>& PlayerList = GetOuterUEngine()->GetGamePlayers(this);

	for (int32 PlayerIdx = 0; PlayerIdx < PlayerList.Num(); PlayerIdx++)
	{
		if (SplitType < SplitscreenInfo.Num() && PlayerIdx < SplitscreenInfo[SplitType].PlayerData.Num())
		{
			PlayerList[PlayerIdx]->Size.X = SplitscreenInfo[SplitType].PlayerData[PlayerIdx].SizeX;
			PlayerList[PlayerIdx]->Size.Y = SplitscreenInfo[SplitType].PlayerData[PlayerIdx].SizeY;
			PlayerList[PlayerIdx]->Origin.X = SplitscreenInfo[SplitType].PlayerData[PlayerIdx].OriginX;
			PlayerList[PlayerIdx]->Origin.Y = SplitscreenInfo[SplitType].PlayerData[PlayerIdx].OriginY;
		}
		else
		{
			PlayerList[PlayerIdx]->Size.X = 0.f;
			PlayerList[PlayerIdx]->Size.Y = 0.f;
			PlayerList[PlayerIdx]->Origin.X = 0.f;
			PlayerList[PlayerIdx]->Origin.Y = 0.f;
		}
	}
}
