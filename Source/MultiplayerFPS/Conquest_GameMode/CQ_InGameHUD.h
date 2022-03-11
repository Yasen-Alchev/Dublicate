#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "CQ_InGameHUD.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_InGameHUD : public AMultiplayerFPSInGameHUD
{
	GENERATED_BODY()

public:
	ACQ_InGameHUD();

	virtual void BeginPlay() override;
};
