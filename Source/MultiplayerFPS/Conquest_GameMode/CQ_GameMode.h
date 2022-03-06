// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"
#include "CQ_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ACQ_GameMode : public AMultiplayerFPSGameMode
{
	GENERATED_BODY()
public:
    ACQ_GameMode();

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

};



