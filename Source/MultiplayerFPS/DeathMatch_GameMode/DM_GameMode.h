#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DM_GameMode.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ADM_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADM_GameMode();

    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

    virtual void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    UPROPERTY()
        UUserWidget* CurrentWidget;


    UFUNCTION()
        virtual void StartingGame();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

public:
    UPROPERTY()
        bool bStarted;

    UPROPERTY()
        int redPlayers;

    UPROPERTY()
        int bluePlayers;

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

};
