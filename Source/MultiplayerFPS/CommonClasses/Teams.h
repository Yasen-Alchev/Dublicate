#pragma once

#include "Teams.generated.h"

UENUM(BlueprintType)
enum ETeams
{
	TEAM_NONE		UMETA(DisplayName = "Team None"),
	TEAM_BLUE		UMETA(DisplayName = "Team Blue"),
	TEAM_RED		UMETA(DisplayName = "Team Red")
};

