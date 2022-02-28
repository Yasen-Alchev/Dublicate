// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DM_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ADM_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADM_GameMode();

    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
        UUserWidget* CurrentWidget;

private:

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

public:

    FTimerHandle GameTimer;

    void UpdateGlobalGameTimer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int minutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int seconds;

    UFUNCTION()
        void UpdateObjectiveStats();

};
