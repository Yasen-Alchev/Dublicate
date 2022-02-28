// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/HUD.h"
#include "DM_InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ADM_InGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADM_InGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void UpdateLeaderBoard(int Kills, int Deaths);

	UFUNCTION()
		void UpdateObjectiveStats(ADM_PlayerState* Player1, ADM_PlayerState* Player2, ADM_PlayerState* Player3);

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		void ResetLeaderBoard();

	UFUNCTION()
		void ResetObjectiveStats();

	UFUNCTION()
		void SetOptionMenuVisibility(bool Visibility);

	UFUNCTION()
		void ToggleLeaderBoardVisibility();

	UFUNCTION()
		void GameEnded(const FString& WinnerName);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> GameTimeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> LeaderBoardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> ObjectiveStatsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> EndGameScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> BuyMenuWidgetClass;

private:
	UPROPERTY()
	class UDM_LeaderBoardWidget* LeaderBoardWidget;
	UPROPERTY()
	class UDM_BuyMenuWidget* BuyMenuWidget;
	UPROPERTY()
	class UFPS_MultiplayGameTimeWidget* GameTimeWidget;
	UPROPERTY()
	class UFPS_MultiplayInGameMenuWidget* InGameMenuWidget;
	UPROPERTY()
	class UDM_ObjectiveStatsWidget* ObjectiveStatsWidget;
	UPROPERTY()
	class UDM_EndGameScreenWidget* EndGameScreenWidget;
};
