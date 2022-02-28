// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "GameFramework/HUD.h"
#include "FPS_MultiplayInGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API AFPS_MultiplayInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPS_MultiplayInGameHUD();

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
		virtual void GameEnded(EFPS_Teams WinnerTeam);

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
		class UFPS_MultiplayLeaderBoardWidget* LeaderBoardWidget;

	UPROPERTY()
		class UFPS_MultiplayBuyMenuWidget* BuyMenuWidget;

	UPROPERTY()
		class UFPS_MultiplayGameTimeWidget* GameTimeWidget;

	UPROPERTY()
		class UFPS_MultiplayInGameMenuWidget* InGameMenuWidget;

	UPROPERTY()
		class UFPS_MultiplayObjectiveStatsWidget* ObjectiveStatsWidget;

	UPROPERTY()
		class UFPS_MultiplayEndGameScreenWidget* EndGameScreenWidget;
};
