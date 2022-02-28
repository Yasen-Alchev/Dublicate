// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_MultiplayObjectiveStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayObjectiveStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPS_MultiplayObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

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
