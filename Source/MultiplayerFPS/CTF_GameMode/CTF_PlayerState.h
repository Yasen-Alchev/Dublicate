#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedPlayerState.h"
#include "CTF_PlayerState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACTF_PlayerState : public ATeamBasedPlayerState
{
	GENERATED_BODY()

public:
	ACTF_PlayerState();

};

