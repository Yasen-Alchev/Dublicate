#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "DM_PlayerController.generated.h"


UCLASS()
class MULTIPLAYERFPS_API ADM_PlayerController : public AMultiplayerFPSPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};