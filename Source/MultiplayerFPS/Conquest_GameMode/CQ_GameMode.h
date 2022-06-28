#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"
#include "CQ_GameMode.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_GameMode : public AMultiplayerFPSGameMode
{
	GENERATED_BODY()
public:
    ACQ_GameMode();

protected:
    virtual void BeginPlay() override;

};



