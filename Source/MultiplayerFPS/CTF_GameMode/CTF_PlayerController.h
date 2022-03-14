#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedPlayerController.h"
#include "CTF_PlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACTF_PlayerController : public ATeamBasedPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
