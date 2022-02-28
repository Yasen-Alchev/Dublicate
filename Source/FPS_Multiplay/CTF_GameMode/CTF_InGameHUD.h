// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayInGameHUD.h"
#include "CTF_InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACTF_InGameHUD : public AFPS_MultiplayInGameHUD
{
	GENERATED_BODY()

public:
	ACTF_InGameHUD();

	virtual void BeginPlay() override;

};
