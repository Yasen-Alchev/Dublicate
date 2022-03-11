// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ULeaderBoardUnitWidget> LeaderBoardUnitRef;

private:
	void InitLeaderBoardColumns();
};
