// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStartPoint.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API APlayerStartPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
	TEnumAsByte<ETeams> Team;

};
