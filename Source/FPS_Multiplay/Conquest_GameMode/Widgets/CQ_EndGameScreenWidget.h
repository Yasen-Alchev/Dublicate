// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "FPS_Multiplay/FPS_Default/Widgets/FPS_MultiplayEndGameScreenWidget.h"
#include "CQ_EndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UCQ_EndGameScreenWidget : public UFPS_MultiplayEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UCQ_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


};
