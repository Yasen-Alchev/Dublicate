// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"
#include "CQ_GameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ACQ_GameState : public AMultiplayerFPSGameState
{
	GENERATED_BODY()

public:
	ACQ_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};


