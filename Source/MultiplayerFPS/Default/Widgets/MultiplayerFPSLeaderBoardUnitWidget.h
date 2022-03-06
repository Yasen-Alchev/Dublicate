// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "MultiplayerFPSLeaderBoardUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSLeaderBoardUnitWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UMultiplayerFPSLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerKills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerDeaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_PlayerScore;

	UFUNCTION()
		virtual void SetPlayerName(FString Name);

	UFUNCTION()
		virtual void SetPlayerKills(FString Kills);

	UFUNCTION()
		virtual void SetPlayerDeaths(FString Deaths);

	UFUNCTION()
		virtual void SetPlayerScore(FString Score);

};
