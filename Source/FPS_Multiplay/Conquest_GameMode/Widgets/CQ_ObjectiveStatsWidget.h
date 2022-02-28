// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_Default/Widgets/FPS_MultiplayObjectiveStatsWidget.h"
#include "CQ_ObjectiveStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UCQ_ObjectiveStatsWidget : public UFPS_MultiplayObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UCQ_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
