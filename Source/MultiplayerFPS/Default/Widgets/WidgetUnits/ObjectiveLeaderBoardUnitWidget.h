// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeaderBoardUnitWidget.h"
#include "ObjectiveLeaderBoardUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UObjectiveLeaderBoardUnitWidget : public ULeaderBoardUnitWidget
{
	GENERATED_BODY()

public:

	UObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void SetPlayerObjectiveScore(FString Score);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerObjectiveScore;

};
