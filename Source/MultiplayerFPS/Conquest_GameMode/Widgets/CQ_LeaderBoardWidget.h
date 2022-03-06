// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/MultiplayerFPSLeaderBoardWidget.h"
#include "CQ_LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCQ_LeaderBoardWidget : public UMultiplayerFPSLeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCQ_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

};
