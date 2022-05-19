#include "DM_GameState.h"

#include "DM_PlayerController.h"
#include "DM_PlayerState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"

ADM_GameState::ADM_GameState() {}

void ADM_GameState::BeginPlay()
{
	Super::BeginPlay();
}

void ADM_GameState::GameEnded()
{
	Super::GameEnded();

	ADM_PlayerState* WinnerPlayerState = Cast<ADM_PlayerState>(PlayerArray[0]);
	if (!IsValid(WinnerPlayerState))
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> WinnerPlayerState is not Valid !!!"));
		return;
	}

	int mostKills = WinnerPlayerState->GetPlayerKills();

	for (APlayerState* CurrentPlayerStateVar : PlayerArray)
	{
		ADM_PlayerState* PlayerStateVar = Cast<ADM_PlayerState>(CurrentPlayerStateVar);
		if (IsValid(PlayerStateVar))
		{
			if (PlayerStateVar->GetPlayerKills() > mostKills)
			{
				WinnerPlayerState = PlayerStateVar;
				mostKills = WinnerPlayerState->GetPlayerKills();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> PlayerStateVar is not Valid !!!"));
		}
	}

	UE_LOG(LogTemp, Error, TEXT("ADM_GameState::GameEnded() -> mostKills = %d !!!"), mostKills);

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



