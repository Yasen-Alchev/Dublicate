// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ULeaderBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	ULeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual  void GenerateLeaderBoard();

	UFUNCTION()
		virtual void UpdateLeaderBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ULeaderBoardUnitWidget> LeaderBoardUnitRef;

private:

	UPROPERTY()
	TMap<int32, int32> PlayerToWidgetMap;

	void InitLeaderBoardColumns();
};
