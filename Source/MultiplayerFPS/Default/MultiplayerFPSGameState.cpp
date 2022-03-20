#include "MultiplayerFPSGameState.h"

#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSPlayerState.h"
#include "Net/UnrealNetwork.h"

AMultiplayerFPSGameState::AMultiplayerFPSGameState()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	PlayerRespawnTime = 5;
}

void AMultiplayerFPSGameState::ResetStats() { /*No stats to reset from the base class yet*/ }

void AMultiplayerFPSGameState::UpdateGameTime(int minutes, int seconds)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientUpdateGameTime(minutes, seconds);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::UpdateGameTime(int minutes, int seconds) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::UpdateGameTime(int minutes, int seconds) -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::SetGlobalGameMessage(const FString& Message)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientSetGlobalGameMessage(Message);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::SetGlobalGameMessage(FString Message) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::SetGlobalGameMessage(FString Message) -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::ClearGlobalGameMessage()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientClearGlobalGameMessage();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::ClearGlobalGameMessage() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::ClearGlobalGameMessage() -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::GameEnded()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->ClientEndGame("The Player with the most Points Wins !!!");
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::GameEnded() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::GameEnded() -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::UpdateObjectiveStats()
{
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

void AMultiplayerFPSGameState::RespawnPlayers(bool instant)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->RespawnPlayer(instant);;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::RespawnPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::RespawnPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::KillPlayers()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->KillPlayer();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::KillPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::KillPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void AMultiplayerFPSGameState::DisablePlayersControls(bool bDisable)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->DisableControls(bDisable);
				if (bDisable)
				{
					PlayerController->SetInputMode(FInputModeUIOnly());
				}
				else
				{
					PlayerController->SetInputMode(FInputModeGameOnly());
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::DisablePlayersControls(bool bDisable) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::DisablePlayersControls(bool bDisable) -> PlayerState is not Valid !!!"));
		}
	}
}



