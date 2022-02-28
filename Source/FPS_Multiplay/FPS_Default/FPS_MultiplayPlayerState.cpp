// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayPlayerState.h"

#include "FPS_MultiplayInGameHUD.h"
#include "FPS_MultiplayPlayerController.h"
#include "FPS_MultiplayTeamBasedCharacter.h"
#include "Net/UnrealNetwork.h"


AFPS_MultiplayPlayerState::AFPS_MultiplayPlayerState()
{
	bReplicates = true;

	team = TEAM_NONE;
	Kills = 0;
	Deaths = 0;
	PlayerScore = 0;
	FlagsCaptured = 0;
}

void AFPS_MultiplayPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPS_MultiplayPlayerState, team); 
	DOREPLIFETIME(AFPS_MultiplayPlayerState, Kills); 
	DOREPLIFETIME(AFPS_MultiplayPlayerState, Deaths); 
	DOREPLIFETIME(AFPS_MultiplayPlayerState, PlayerScore); 
	DOREPLIFETIME(AFPS_MultiplayPlayerState, FlagsCaptured);
}

void AFPS_MultiplayPlayerState::FlagWasCaptured()
{
	++FlagsCaptured;
	PlayerScore += 10;
	AFPS_MultiplayPlayerController* Controller = Cast<AFPS_MultiplayPlayerController>(GetPawn()->GetController());
	if(IsValid(Controller))
	{
		AFPS_MultiplayInGameHUD* PlayerHud =  Cast<AFPS_MultiplayInGameHUD>(Controller->GetHUD());
		if (IsValid(PlayerHud))
		{
			PlayerHud->UpdateLeaderBoardStats();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::FlagWasCaptured() -> PlayerHud is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerState::FlagWasCaptured() -> Controller is not Valid !!!"));
	}
}


