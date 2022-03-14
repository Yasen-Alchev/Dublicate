#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSInGameHUD.h"
#include "DM_InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ADM_InGameHUD : public AMultiplayerFPSInGameHUD
{
	GENERATED_BODY()

public:
	ADM_InGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
