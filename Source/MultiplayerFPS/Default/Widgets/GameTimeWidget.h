// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UGameTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UGameTimeWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		void SetGlobalGameMessage(FString Message);

	UFUNCTION()
		void ClearGlobalGameMessage();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_GlobalGameMessage;
};
