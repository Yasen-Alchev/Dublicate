// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/MultiplayerFPSLeaderBoardWidget.h"
#include "CTF_LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCTF_LeaderBoardWidget : public UMultiplayerFPSLeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCTF_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
