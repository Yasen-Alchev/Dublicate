// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPSObjectiveStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSObjectiveStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMultiplayerFPSObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void UpdateStats(int RedScore, int BlueScore);

	UFUNCTION()
		virtual void ResetStats();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_RedScoreStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_BlueScoreStat;

};
