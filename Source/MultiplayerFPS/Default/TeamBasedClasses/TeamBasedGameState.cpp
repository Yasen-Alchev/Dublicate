#include "TeamBasedGameState.h"

#include "TeamBasedPlayerController.h"
#include "TeamBasedPlayerState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "Net/UnrealNetwork.h"

ATeamBasedGameState::ATeamBasedGameState()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	MaxFlagsToCapture = 2;
	PlayerRespawnTime = 5;
}

void ATeamBasedGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamBasedGameState, BlueTeamCapturedFlags);
	DOREPLIFETIME(ATeamBasedGameState, RedTeamCapturedFlags);
}

void ATeamBasedGameState::ResetStats()
{
	RedTeamCapturedFlags = 0;
	BlueTeamCapturedFlags = 0;
}

void ATeamBasedGameState::RedFlagCaptured()
{
	++RedTeamCapturedFlags;
	UpdateObjectiveStats();
	if (RedTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

void ATeamBasedGameState::BlueFlagCaptured()
{
	++BlueTeamCapturedFlags;
	UpdateObjectiveStats();
	if (BlueTeamCapturedFlags == MaxFlagsToCapture)
	{
		GameEnded();
	}
}

void ATeamBasedGameState::GameEnded()
{
	Super::GameEnded();

	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ATeamBasedPlayerState* PlayerState = Cast<ATeamBasedPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ATeamBasedPlayerController* PlayerController = Cast<ATeamBasedPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				FString Winner = "";
				if (BlueTeamCapturedFlags > RedTeamCapturedFlags)
				{
					Winner = "Blue Team Wins !";
				}
				else if (BlueTeamCapturedFlags < RedTeamCapturedFlags)
				{
					Winner = "Red Team Wins !";
				}
				else
				{
					Winner = "Draw";
				}
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->ClientEndGame(Winner);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ATeamBasedGameState::GameEnded() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ATeamBasedGameState::GameEnded() -> PlayerState is not Valid !!!"));
		}
	}
}

void ATeamBasedGameState::UpdateLeaderBoards()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ATeamBasedPlayerState* PlayerState = Cast<ATeamBasedPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ATeamBasedPlayerController* PlayerController = Cast<ATeamBasedPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientUpdateLeaderBoardStats();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ATeamBasedGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
		}
	}
}

void ATeamBasedGameState::UpdateObjectiveStats()
{
	Super::UpdateObjectiveStats();

	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ATeamBasedPlayerState* PlayerState = Cast<ATeamBasedPlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ATeamBasedPlayerController* PlayerController = Cast<ATeamBasedPlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				TArray<FString> ObjectiveStats;
				ObjectiveStats.Emplace(FString("Red:" + FString::FromInt(RedTeamCapturedFlags)));
				ObjectiveStats.Emplace(FString("Blue:" + FString::FromInt(BlueTeamCapturedFlags)));

				PlayerController->ClientUpdateObjectiveStats(ObjectiveStats);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"), *PlayerState->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ATeamBasedGameState::UpdateObjectiveStats() -> PlayerState is not Valid !!!"));
		}
	}
}

