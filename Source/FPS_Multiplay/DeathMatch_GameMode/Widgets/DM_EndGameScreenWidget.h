// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "DM_EndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UDM_EndGameScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UDM_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void ReturnToMainMenu();

	UFUNCTION()
	void SetWinner(const FString& WinnerName);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_EndGameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_EndGameWinnerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_EndGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_ReturnToMainMenu;
};
