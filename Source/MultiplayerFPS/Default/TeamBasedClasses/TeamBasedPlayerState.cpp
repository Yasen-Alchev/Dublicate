#include "TeamBasedPlayerState.h"

#include "TeamBasedCharacter.h"
#include "TeamBasedGameMode.h"
#include "TeamBasedGameState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "Net/UnrealNetwork.h"


ATeamBasedPlayerState::ATeamBasedPlayerState()
{
	Team = TEAM_NONE;
	FlagsCaptured = 0;
}

void ATeamBasedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamBasedPlayerState, Team);
	DOREPLIFETIME(ATeamBasedPlayerState, FlagsCaptured);
}

void ATeamBasedPlayerState::FlagWasCaptured()
{
	++FlagsCaptured;
	PlayerScore += 10;

	GetWorldTimerManager().SetTimer(TimerHandle, this,  &ATeamBasedPlayerState::ServerUpdateLeaderBoards, 2, false);

	// TODO: REM
	//PlayerHud->ToggleLeaderBoardVisibility();
}

void ATeamBasedPlayerState::ServerUpdateLeaderBoards_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedPlayerState::FlagWasCaptured -> World is not Valid !!!"));
		return;
	}

	AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedPlayerState::FlagWasCaptured -> GameMode is not Valid !!!"));
		return;
	}

	ATeamBasedGameState* GameState = GameMode->GetGameState<ATeamBasedGameState>();
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedPlayerState::FlagWasCaptured -> GameState is not Valid !!!"));
		return;
	}
	GameState->UpdateLeaderBoards();
}



