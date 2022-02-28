// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayPlayerState.h"
#include "CQ_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACQ_PlayerState : public AFPS_MultiplayPlayerState
{
	GENERATED_BODY()

public:

	ACQ_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};