#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/HUD.h"
#include "DM_InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ADM_InGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADM_InGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void UpdateStats(int Kills, int Deaths);

	UFUNCTION()
		void UpdateObjectiveStats(ADM_PlayerState* Player1, ADM_PlayerState* Player2, ADM_PlayerState* Player3);

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		void ResetStats();

	UFUNCTION()
		void ResetObjectiveStats();

	UFUNCTION()
		void SetOptionsMenuVisibility(bool Visibility);

	UFUNCTION()
		void ToggleStatsVisibility();

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
	class UGameTimeWidget* GameTimeWidget;
	UPROPERTY()
	class UInGameMenuWidget* InGameMenuWidget;
	UPROPERTY()
	class UDM_ObjectiveStatsWidget* ObjectiveStatsWidget;
	UPROPERTY()
	class UDM_EndGameScreenWidget* EndGameScreenWidget;
};
