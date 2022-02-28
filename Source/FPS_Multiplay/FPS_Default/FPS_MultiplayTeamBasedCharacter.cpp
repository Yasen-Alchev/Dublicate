// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_MultiplayTeamBasedCharacter.h"

#include "FPS_MultiplayGameInstance.h"
#include "FPS_MultiplayPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// AFPS_MultiplayTeamBasedCharacter

AFPS_MultiplayTeamBasedCharacter::AFPS_MultiplayTeamBasedCharacter()
{
	myMaterial = nullptr;
	objectiveScore = 0;
}

void AFPS_MultiplayTeamBasedCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaterialChange();
}

void AFPS_MultiplayTeamBasedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPS_MultiplayTeamBasedCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPS_MultiplayTeamBasedCharacter, objectiveScore);
}

void AFPS_MultiplayTeamBasedCharacter::InitTeam()
{
	Super::InitTeam();
	
	MaterialChange();
}

void AFPS_MultiplayTeamBasedCharacter::MaterialChange()
{
	if (Team != TEAM_NONE)
	{
		if (Team == TEAM_BLUE)
		{
			myMaterial = GetMesh()->CreateDynamicMaterialInstance(0, TeamBlueSkin);
		}
		else
		{
			myMaterial = GetMesh()->CreateDynamicMaterialInstance(0, TeamRedSkin);
		}
		if (IsValid(myMaterial))
		{
			GetMesh()->SetMaterial(0, myMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayTeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"), *PlayerName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayTeamBasedCharacter::MaterialChange() -> Team is not Valid!!!"), *PlayerName);
	}
}

