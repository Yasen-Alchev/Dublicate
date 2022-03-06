// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPSLeaderBoardUnitWidget.h"
#include "MultiplayerFPSObjectiveLeaderBoardUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSObjectiveLeaderBoardUnitWidget : public UMultiplayerFPSLeaderBoardUnitWidget
{
	GENERATED_BODY()

public:

	UMultiplayerFPSObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void SetPlayerObjectiveScore(FString Score);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerObjectiveScore;

};
