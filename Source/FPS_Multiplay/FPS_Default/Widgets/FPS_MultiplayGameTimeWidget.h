// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPS_MultiplayGameTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API UFPS_MultiplayGameTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFPS_MultiplayGameTimeWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_GameTime;
};
