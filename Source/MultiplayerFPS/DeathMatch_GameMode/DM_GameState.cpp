#include "DM_GameState.h"

#include "DM_PlayerController.h"
#include "DM_PlayerState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"

ADM_GameState::ADM_GameState()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	MaxFlagsToCapture = 2;
	PlayerRespawnTime = 5;
}

void ADM_GameState::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerArray.Num() > 2)
	{
		Player1 = Cast<ADM_PlayerState>(PlayerArray[0]);
		Player2 = Cast<ADM_PlayerState>(PlayerArray[1]);
		Player3 = Cast<ADM_PlayerState>(PlayerArray[2]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameState::BeginPlay() -> PlayerArray.Num is less than 3 !!!"));
	}
}

void ADM_GameState::UpdateLeaders()
{
	int First_Kills = Player1->GetKills();
	int Second_Kills = Player2->GetKills();
	int Third_Kills = Player3->GetKills();

	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		int CurrentPlayerKills = PlayerState->GetKills();

		if(CurrentPlayerKills > First_Kills)
		{
			Player3 = Player2;
			Player2 = Player1;
			Player1 = PlayerState;
		}
		else if(CurrentPlayerKills > Second_Kills)
		{
			Player3 = Player2;
			Player2 = PlayerState;
		}
		else if (CurrentPlayerKills > Third_Kills)
		{
			Player3 = PlayerState;
		}
		else
		{
			continue;
		}
		First_Kills = Player1->GetKills();
		Second_Kills = Player2->GetKills();
		Third_Kills = Player3->GetKills();
	}
}

void ADM_GameState::UpdateObjectiveStats()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->UpdateObjectiveStats(Player1, Player2, Player3);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::UpdateObjectiveStats() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::UpdateObjectiveStats()->PlayerState is not Valid !!!"));
		}
	}
}

void ADM_GameState::UpdateGameTime(int minutes, int seconds)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->ClientRPCUpdateGameTime(minutes, seconds);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::UpdateGameTime(int minutes, int seconds) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::UpdateGameTime(int minutes, int seconds) -> PlayerState is not Valid !!!"));
		}
	}
}

void ADM_GameState::GameEnded()
{
	ADM_PlayerController* WinnerController = Cast<ADM_PlayerController>(Player1->GetInstigatorController());
	if (IsValid(WinnerController))
	{
		for (APlayerState* CurrentPlayerState : PlayerArray)
		{
			ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
			if (IsValid(PlayerState))
			{
				ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
				if (IsValid(PlayerController))
				{
					PlayerController->SetInputMode(FInputModeUIOnly());
					PlayerController->ClientRPCEndGame(WinnerController->GetName());
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> WinnerController is not Valid !!!"));
	}
}

void ADM_GameState::RespawnPlayers(bool instant)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->RespawnPlayer(instant);;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::RespawnPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::RespawnPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void ADM_GameState::KillPlayers()
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
			if (IsValid(PlayerController))
			{
				PlayerController->KillPlayer();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::KillPlayers() -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::KillPlayers() -> PlayerState is not Valid !!!"));
		}
	}
}

void ADM_GameState::DisablePlayersControls(bool bDisable)
{
	for (APlayerState* CurrentPlayerState : PlayerArray)
	{
		ADM_PlayerState* PlayerState = Cast<ADM_PlayerState>(CurrentPlayerState);
		if (IsValid(PlayerState))
		{
			ADM_PlayerController* PlayerController = Cast<ADM_PlayerController>(PlayerState->GetOwner());
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
				UE_LOG(LogTemp, Error, TEXT("ADM_GameState::DisablePlayersControls(bool bDisable) -> PlayerController is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::DisablePlayersControls(bool bDisable) -> PlayerState is not Valid !!!"));
		}
	}
}



