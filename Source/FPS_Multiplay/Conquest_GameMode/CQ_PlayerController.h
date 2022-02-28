// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayPlayerController.h"
#include "CQ_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACQ_PlayerController : public AFPS_MultiplayPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
