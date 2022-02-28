// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "FPS_MultiplayInGameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayInGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFPS_MultiplayInGameMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ResumeButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXTBlock_GameOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_ResumeGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_QuitGame;

};
