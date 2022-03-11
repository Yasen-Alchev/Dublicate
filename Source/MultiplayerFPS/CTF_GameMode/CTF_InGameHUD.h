#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "CTF_InGameHUD.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACTF_InGameHUD : public AMultiplayerFPSInGameHUD
{
	GENERATED_BODY()

public:
	ACTF_InGameHUD();

	virtual void BeginPlay() override;

};
