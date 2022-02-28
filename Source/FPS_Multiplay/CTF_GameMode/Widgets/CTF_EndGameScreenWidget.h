// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "FPS_Multiplay/FPS_Default/Widgets/FPS_MultiplayEndGameScreenWidget.h"
#include "CTF_EndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UCTF_EndGameScreenWidget : public UFPS_MultiplayEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UCTF_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
