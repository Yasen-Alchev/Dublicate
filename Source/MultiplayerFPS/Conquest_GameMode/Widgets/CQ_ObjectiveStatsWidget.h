// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/MultiplayerFPSObjectiveStatsWidget.h"
#include "CQ_ObjectiveStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCQ_ObjectiveStatsWidget : public UMultiplayerFPSObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UCQ_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
