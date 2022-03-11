#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "CTF_PlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACTF_PlayerController : public AMultiplayerFPSPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
