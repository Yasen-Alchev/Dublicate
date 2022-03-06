// Fill out your copyright notice in the Description page of Project Settings.


#include "CQ_PlayerState.h"

#include "CQ_Character.h"
#include "CQ_GameState.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_PlayerState.h"
#include "Net/UnrealNetwork.h"


ACQ_PlayerState::ACQ_PlayerState()
{
	bReplicates = true;
	team = TEAM_NONE;
	FlagsCaptured = 0;
}

void ACQ_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACQ_PlayerState, team); 
	DOREPLIFETIME(ACQ_PlayerState, FlagsCaptured);
}
