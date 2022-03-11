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
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

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
