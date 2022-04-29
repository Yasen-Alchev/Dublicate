#include "TeamBasedPlayerState.h"

#include "TeamBasedPlayerController.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
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

	ATeamBasedPlayerController* Controller = Cast<ATeamBasedPlayerController>(GetPawn()->GetController());
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedPlayerState::FlagWasCaptured() -> Controller is not Valid !!!"));
		return;
	}

	AMultiplayerFPSInGameHUD* PlayerHud = Cast<AMultiplayerFPSInGameHUD>(Controller->GetHUD());
	if (!IsValid(PlayerHud))
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamBasedPlayerState::FlagWasCaptured() -> PlayerHud is not Valid !!!"));
		return;
	}
	PlayerHud->UpdateLeaderBoardStats();

	// TODO: REM
	//PlayerHud->ToggleLeaderBoardVisibility();

}


