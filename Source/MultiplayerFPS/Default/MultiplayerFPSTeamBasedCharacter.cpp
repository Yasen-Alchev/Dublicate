// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerFPSTeamBasedCharacter.h"

#include "MultiplayerFPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// AMultiplayerFPSTeamBasedCharacter

AMultiplayerFPSTeamBasedCharacter::AMultiplayerFPSTeamBasedCharacter()
{
	myMaterial = nullptr;
	objectiveScore = 0;
}

void AMultiplayerFPSTeamBasedCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaterialChange();
}

void AMultiplayerFPSTeamBasedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerFPSTeamBasedCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSTeamBasedCharacter, objectiveScore);
}

void AMultiplayerFPSTeamBasedCharacter::InitTeam()
{
	Super::InitTeam();
	
	MaterialChange();
}

void AMultiplayerFPSTeamBasedCharacter::MaterialChange()
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
			UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSTeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"), *PlayerName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSTeamBasedCharacter::MaterialChange() -> Team is not Valid!!!"), *PlayerName);
	}
}

