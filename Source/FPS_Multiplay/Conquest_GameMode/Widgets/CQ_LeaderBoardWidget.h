// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_Default/Widgets/FPS_MultiplayLeaderBoardWidget.h"
#include "CQ_LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UCQ_LeaderBoardWidget : public UFPS_MultiplayLeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCQ_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

};
