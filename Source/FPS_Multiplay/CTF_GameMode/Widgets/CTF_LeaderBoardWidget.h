// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_Default/Widgets/FPS_MultiplayLeaderBoardWidget.h"
#include "CTF_LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UCTF_LeaderBoardWidget : public UFPS_MultiplayLeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCTF_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
