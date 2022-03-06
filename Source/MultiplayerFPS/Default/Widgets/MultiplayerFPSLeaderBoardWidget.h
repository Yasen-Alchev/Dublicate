// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "MultiplayerFPSLeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSLeaderBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UMultiplayerFPSLeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual  void GenerateLeaderBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UMultiplayerFPSLeaderBoardUnitWidget> LeaderBoardUnitRef;

private:
	void InitLeaderBoardColumns();
};
