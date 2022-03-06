// Fill out your copyright notice in the Description page of Project Settings.


#include "DM_PlayerState.h"
#include "Net/UnrealNetwork.h"

ADM_PlayerState::ADM_PlayerState()
{
	ResetStats();
}

void ADM_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADM_PlayerState, kills);
	DOREPLIFETIME(ADM_PlayerState, deaths);
}

void ADM_PlayerState::ResetStats()
{
	kills = 0;
	deaths = 0;
}

