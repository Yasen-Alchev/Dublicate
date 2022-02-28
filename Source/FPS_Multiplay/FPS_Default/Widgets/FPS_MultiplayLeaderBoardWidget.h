// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "FPS_MultiplayLeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayLeaderBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFPS_MultiplayLeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual  void GenerateLeaderBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UFPS_MultiplayLeaderBoardUnitWidget> LeaderBoardUnitRef;

private:
	void InitLeaderBoardColumns();
};
