#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "CQ_PlayerState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_PlayerState : public AMultiplayerFPSPlayerState
{
	GENERATED_BODY()

public:

	ACQ_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};