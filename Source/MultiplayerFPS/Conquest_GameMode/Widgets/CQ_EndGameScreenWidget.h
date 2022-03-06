// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MultiplayerFPS/Default/Widgets/MultiplayerFPSEndGameScreenWidget.h"
#include "CQ_EndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCQ_EndGameScreenWidget : public UMultiplayerFPSEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UCQ_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


};
