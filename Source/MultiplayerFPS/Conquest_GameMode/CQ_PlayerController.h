#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedPlayerController.h"
#include "CQ_PlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_PlayerController : public ATeamBasedPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
};
