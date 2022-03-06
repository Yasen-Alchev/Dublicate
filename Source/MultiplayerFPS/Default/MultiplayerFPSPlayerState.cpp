// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSPlayerState.h"

#include "MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSTeamBasedCharacter.h"
#include "Net/UnrealNetwork.h"


AMultiplayerFPSPlayerState::AMultiplayerFPSPlayerState()
{
	bReplicates = true;

	team = TEAM_NONE;
	Kills = 0;
	Deaths = 0;
	PlayerScore = 0;
	FlagsCaptured = 0;
}

void AMultiplayerFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSPlayerState, team); 
	DOREPLIFETIME(AMultiplayerFPSPlayerState, Kills); 
	DOREPLIFETIME(AMultiplayerFPSPlayerState, Deaths); 
	DOREPLIFETIME(AMultiplayerFPSPlayerState, PlayerScore); 
	DOREPLIFETIME(AMultiplayerFPSPlayerState, FlagsCaptured);
}

void AMultiplayerFPSPlayerState::FlagWasCaptured()
{
	++FlagsCaptured;
	PlayerScore += 10;
	AMultiplayerFPSPlayerController* Controller = Cast<AMultiplayerFPSPlayerController>(GetPawn()->GetController());
	if(IsValid(Controller))
	{
		AMultiplayerFPSInGameHUD* PlayerHud =  Cast<AMultiplayerFPSInGameHUD>(Controller->GetHUD());
		if (IsValid(PlayerHud))
		{
			PlayerHud->UpdateLeaderBoardStats();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerState::FlagWasCaptured() -> PlayerHud is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerState::FlagWasCaptured() -> Controller is not Valid !!!"));
	}
}


