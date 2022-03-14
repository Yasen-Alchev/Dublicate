#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"
#include "DM_GameMode.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ADM_GameMode : public AMultiplayerFPSGameMode
{
	GENERATED_BODY()

protected:

	ADM_GameMode();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;
};
