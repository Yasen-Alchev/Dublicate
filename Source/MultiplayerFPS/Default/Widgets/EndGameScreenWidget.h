// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "EndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UEndGameScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UEndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		virtual void QuitButtonClicked();

	UFUNCTION()
		virtual void ReturnToMainMenu();

	UFUNCTION()
		virtual void SetWinnerTeam(const FString& WinnerTeam);

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
