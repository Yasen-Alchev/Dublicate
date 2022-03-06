// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSGameState.h"

#include "MultiplayerFPSTeamBasedCharacter.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSPlayerState.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "Net/UnrealNetwork.h"

AMultiplayerFPSGameState::AMultiplayerFPSGameState()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	MaxFlagsToCapture = 2;
	PlayerRespawnTime = 5;
}

void AMultiplayerFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSGameState, BlueTeamCapturedFlags);
	DOREPLIFETIME(AMultiplayerFPSGameState, RedTeamCapturedFlags);
}

void AMultiplayerFPSGameState::ResetStats()
{
	RedTeamCapturedFlags = 0;
	BlueTeamCapturedFlags = 0;
}

void AMultiplayerFPSGameState::RedFlagCaptured()
{
	UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::BlueFlagCaptured() -> Red TeamCapturedFlags++  !!!"));
	++RedTeamCapturedFlags;
	UpdateObjectiveStats();
	if (RedTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

void AMultiplayerFPSGameState::BlueFlagCaptured()
{
	UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::BlueFlagCaptured() -> Blue TeamCapturedFlags++  !!!"));
	++BlueTeamCapturedFlags;
	UpdateObjectiveStats();
	if (BlueTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

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
				TEnumAsByte<ETeams> WinnerTeam;
				if (BlueTeamCapturedFlags > RedTeamCapturedFlags)
				{
					WinnerTeam = TEAM_BLUE;
				}
				else if (BlueTeamCapturedFlags < RedTeamCapturedFlags)
				{
					WinnerTeam = TEAM_RED;
				}
				else
				{
					WinnerTeam = TEAM_NONE;
				}
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->ClientEndGame(WinnerTeam);
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
				PlayerController->ClientUpdateObjectiveStats(RedTeamCapturedFlags, BlueTeamCapturedFlags);
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

//void AMultiplayerFPSGameState::OnRep_UpdateObjectiveStats()
//{
//	UE_LOG(LogTemp, Error, TEXT("OnRepNotify Called!"));
//	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, FString::Printf(TEXT("OnRepNotify Called!")));
//	for (APlayerState* CurrentPlayerState : PlayerArray)
//	{
//		AMultiplayerFPSPlayerState* PlayerState = Cast<AMultiplayerFPSPlayerState>(CurrentPlayerState);
//		if (IsValid(PlayerState))
//		{
//			AMultiplayerFPSPlayerController* PlayerController = Cast<AMultiplayerFPSPlayerController>(PlayerState->GetOwner());
//			if (IsValid(PlayerController))
//			{
//				PlayerController->ClientRPCUpdateObjectiveStats(RedTeamCapturedFlags, BlueTeamCapturedFlags);
//			}
//			else
//			{
//				UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
//		}
//	}
//}

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



