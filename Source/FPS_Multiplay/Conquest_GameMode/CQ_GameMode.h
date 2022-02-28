// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayGameMode.h"
#include "CQ_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACQ_GameMode : public AFPS_MultiplayGameMode
{
	GENERATED_BODY()
public:
    ACQ_GameMode();

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

};



