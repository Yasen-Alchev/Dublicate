// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerFPSGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerFPSGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

    AMultiplayerFPSGameMode();

    /** Called when the game starts. */
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

	/** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
        UUserWidget* CurrentWidget;

    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        virtual void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

    UFUNCTION()
       virtual void StartingGame();

    UFUNCTION()
		virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    UFUNCTION()
		virtual void PostLogin(APlayerController* NewPlayer) override;

    UPROPERTY()
        bool bStarted;

    UPROPERTY()
        int redPlayers;

    UPROPERTY()
        int bluePlayers;

public:

    UPROPERTY()
		FTimerHandle GameTimer;

    UPROPERTY()
        FTimerHandle StartingTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int minutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
        int seconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
        int minPlayersToStart;

    UFUNCTION()
        virtual void UpdateGlobalGameTimer(int& min, int& sec);

    UFUNCTION()
        virtual void UpdateObjectiveStats();

    UFUNCTION()
        virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UFUNCTION()
        virtual TEnumAsByte<ETeams> assignTeam();

};



