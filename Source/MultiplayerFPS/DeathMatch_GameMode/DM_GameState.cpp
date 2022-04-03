#include "DM_GameState.h"

#include "DM_PlayerController.h"
#include "DM_PlayerState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"

ADM_GameState::ADM_GameState() {}

void ADM_GameState::BeginPlay()
{
	Super::BeginPlay();
}

void ADM_GameState::UpdateObjectiveStats()
{
	Super::UpdateObjectiveStats();

	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{

				TArray<FString> ObjectiveStats;
				ObjectiveStats.Emplace("Game Has No Leader");
				PlayerController->ClientUpdateObjectiveStats(ObjectiveStats);

			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
		}
	}
}

void ADM_GameState::GameEnded()
{
	Super::GameEnded();

	int mostKills = 0;
	ADM_PlayerState* WinnerPlayerState = nullptr;

	for (APlayerState* CurrentPlayerStateVar : PlayerArray)
	{
		ADM_PlayerState* PlayerStateVar = Cast<ADM_PlayerState>(CurrentPlayerStateVar);
		if (IsValid(PlayerStateVar))
		{
			if(WinnerPlayerState == nullptr)
			{
				WinnerPlayerState = PlayerStateVar;
				mostKills = WinnerPlayerState->getPlayerKills();
			}
			else if (PlayerStateVar->getPlayerKills() > mostKills)
			{
				WinnerPlayerState = PlayerStateVar;
				mostKills = WinnerPlayerState->getPlayerKills();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> PlayerState is not Valid !!!"));
		}
	}

	if(!IsValid(WinnerPlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> WinnerPlayerState is not Valid !!!"));
		return;
	}

	FString WinnerName = WinnerPlayerState->GetPlayerName();
	if(WinnerName.IsEmpty())
	{
		WinnerName = "Unknown Player";
	}

	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->ClientEndGame(WinnerName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> PlayerState is not Valid !!!"));
		}
	}
}



