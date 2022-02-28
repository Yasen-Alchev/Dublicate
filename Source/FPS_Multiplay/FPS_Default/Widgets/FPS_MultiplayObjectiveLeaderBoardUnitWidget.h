// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_MultiplayLeaderBoardUnitWidget.h"
#include "FPS_MultiplayObjectiveLeaderBoardUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayObjectiveLeaderBoardUnitWidget : public UFPS_MultiplayLeaderBoardUnitWidget
{
	GENERATED_BODY()

public:

	UFPS_MultiplayObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void SetPlayerObjectiveScore(FString Score);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerObjectiveScore;

};
