// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/MultiplayerFPSObjectiveStatsWidget.h"
#include "CTF_ObjectiveStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCTF_ObjectiveStatsWidget : public UMultiplayerFPSObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UCTF_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
