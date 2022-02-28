// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayGameState.h"

#include "FPS_MultiplayTeamBasedCharacter.h"
#include "FPS_MultiplayPlayerController.h"
#include "FPS_MultiplayPlayerState.h"
#include "FPS_Multiplay/CTF_GameMode/CTF_GameState.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "Net/UnrealNetwork.h"

AFPS_MultiplayGameState::AFPS_MultiplayGameState()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	MaxFlagsToCapture = 2;
	PlayerRespawnTime = 5;
}

void AFPS_MultiplayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPS_MultiplayGameState, BlueTeamCapturedFlags);
	DOREPLIFETIME(AFPS_MultiplayGameState, RedTeamCapturedFlags);
}

void AFPS_MultiplayGameState::ResetStats()
{
	RedTeamCapturedFlags = 0;
	BlueTeamCapturedFlags = 0;
}

void AFPS_MultiplayGameState::RedFlagCaptured()
{
	UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::BlueFlagCaptured() -> Red TeamCapturedFlags++  !!!"));
	++RedTeamCapturedFlags;
	UpdateObjectiveStats();
	if (RedTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

void AFPS_MultiplayGameState::BlueFlagCaptured()
{
	UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::BlueFlagCaptured() -> Blue TeamCapturedFlags++  !!!"));
	++BlueTeamCapturedFlags;
	UpdateObjectiveStats();
	if (BlueTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

void AFPS_MultiplayGameState::UpdateGameTime(int minutes, int seconds)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientUpdateGameTime(minutes, seconds);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::UpdateGameTime(int minutes, int seconds) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::UpdateGameTime(int minutes, int seconds) -> PlayerState is not Valid !!!"));
		}
	}
}

void AFPS_MultiplayGameState::GameEnded()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				TEnumAsByte<EFPS_Teams> WinnerTeam;
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
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::GameEnded() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::GameEnded() -> PlayerState is not Valid !!!"));
		}
	}
}

void AFPS_MultiplayGameState::UpdateObjectiveStats()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientUpdateObjectiveStats(RedTeamCapturedFlags, BlueTeamCapturedFlags);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
		}
	}
}

//void AFPS_MultiplayGameState::OnRep_UpdateObjectiveStats()
//{
//	UE_LOG(LogTemp, Error, TEXT("OnRepNotify Called!"));
//	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, FString::Printf(TEXT("OnRepNotify Called!")));
//	for (APlayerState* CurrentPlayerState : PlayerArray)
//	{
//		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
//		if (IsValid(PlayerState))
//		{
//			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
//			if (IsValid(PlayerController))
//			{
//				PlayerController->ClientRPCUpdateObjectiveStats(RedTeamCapturedFlags, BlueTeamCapturedFlags);
//			}
//			else
//			{
//				UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
//		}
//	}
//}

void AFPS_MultiplayGameState::RespawnPlayers(bool instant)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->RespawnPlayer(instant);;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::RespawnPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::RespawnPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void AFPS_MultiplayGameState::KillPlayers()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->KillPlayer();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::KillPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::KillPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void AFPS_MultiplayGameState::DisablePlayersControls(bool bDisable)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		AFPS_MultiplayPlayerState* PlayerState = Cast<AFPS_MultiplayPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(PlayerState->GetOwner());
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
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::DisablePlayersControls(bool bDisable) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameState::DisablePlayersControls(bool bDisable) -> PlayerState is not Valid !!!"));
		}
	}
}



