// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "FPS_MultiplayEndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayEndGameScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFPS_MultiplayEndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void QuitButtonClicked();

	UFUNCTION()
		virtual void ReturnToMainMenu();

	UFUNCTION()
		virtual void SetWinnerTeam(EFPS_Teams WinnerTeam);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_EndGameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UTextBlock* TXTBlock_EndGameWinnerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UButton* Button_EndGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, BlueprintProtected))
		class UButton* Button_ReturnToMainMenu;
};
