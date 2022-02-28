// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayGameState.h"
#include "CQ_GameState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACQ_GameState : public AFPS_MultiplayGameState
{
	GENERATED_BODY()

public:
	ACQ_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};


