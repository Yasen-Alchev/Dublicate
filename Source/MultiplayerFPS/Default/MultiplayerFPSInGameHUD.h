#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/WidgetUnits/BuyMenuWeaponUnitWidget.h"
#include "MultiplayerFPSInGameHUD.generated.h"

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
		virtual void UpdateObjectiveStats(const TArray<FString>& ObjectiveStats);

	UFUNCTION()
		virtual void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		virtual void ResetObjectiveStats();

	UFUNCTION()
		virtual void ResetGameLeader();

	UFUNCTION()
		virtual void SetOptionMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void SetBuyMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void SetImageWeaponSlot(uint32 SlotIndex, UTexture2D* WeaponTexture2D);

	UFUNCTION()
		virtual void SelectWeaponSlot(uint32 SlotIndex);

	UFUNCTION()
		virtual void ToggleLeaderBoardVisibility();

	UFUNCTION()
		virtual void UpdateLeaderBoardStats();

	UFUNCTION()
		virtual void SetGlobalGameMessage(FString Message);

	UFUNCTION()
		virtual void SelectWeapon(UBuyMenuWeaponUnitWidget* SelectedWeapon);

	UFUNCTION()
		virtual void ClearGlobalGameMessage();

	UFUNCTION()
		virtual void GameEnded(const FString& WinnerTeam);

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

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> WeaponSlotsWidgetClass;


	UPROPERTY()
		class ULeaderBoardWidget* LeaderBoardWidget;

	UPROPERTY()
		class UWeaponSlotsWidget* WeaponSlotsWidget;

	UPROPERTY()
		class UBuyMenuWidget* BuyMenuWidget;

	UPROPERTY()
		class UGameTimeWidget* GameTimeWidget;

	UPROPERTY()
		class UInGameMenuWidget* InGameMenuWidget;

	UPROPERTY()
		class UObjectiveStatsWidget* ObjectiveStatsWidget;

	UPROPERTY()
		class UEndGameScreenWidget* EndGameScreenWidget;


private:

	UPROPERTY()
			class UTexture2D* CrosshairTex;

};
