// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "GameFramework/PlayerStart.h"
#include "FPS_PlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API AFPS_PlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
	TEnumAsByte<EFPS_Teams> Team;

};
