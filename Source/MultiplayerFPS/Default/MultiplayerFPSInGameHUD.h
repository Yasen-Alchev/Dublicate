// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/HUD.h"
#include "MultiplayerFPSInGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMultiplayerFPSInGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		virtual void UpdateObjectiveStats(int32 RedScore, int32 BlueScore);

	UFUNCTION()
		virtual void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		virtual void ResetObjectiveStats();

	UFUNCTION()
		virtual void SetOptionMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void ToggleLeaderBoardVisibility();

	UFUNCTION()
		virtual void UpdateLeaderBoardStats();

	UFUNCTION()
		virtual void GameEnded(ETeams WinnerTeam);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> GameTimeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> LeaderBoardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> ObjectiveStatsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> EndGameScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> BuyMenuWidgetClass;


	UPROPERTY()
		class UMultiplayerFPSLeaderBoardWidget* LeaderBoardWidget;

	UPROPERTY()
		class UMultiplayerFPSBuyMenuWidget* BuyMenuWidget;

	UPROPERTY()
		class UMultiplayerFPSGameTimeWidget* GameTimeWidget;

	UPROPERTY()
		class UMultiplayerFPSInGameMenuWidget* InGameMenuWidget;

	UPROPERTY()
		class UMultiplayerFPSObjectiveStatsWidget* ObjectiveStatsWidget;

	UPROPERTY()
		class UMultiplayerFPSEndGameScreenWidget* EndGameScreenWidget;
};
