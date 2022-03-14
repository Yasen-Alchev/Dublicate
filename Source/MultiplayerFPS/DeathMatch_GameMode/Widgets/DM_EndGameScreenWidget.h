#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/EndGameScreenWidget.h"
#include "DM_EndGameScreenWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UDM_EndGameScreenWidget : public UEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UDM_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void SetWinnerTeam(const FString& Winner) override;

};
