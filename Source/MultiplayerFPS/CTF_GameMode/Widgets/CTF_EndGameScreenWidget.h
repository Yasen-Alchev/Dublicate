#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "MultiplayerFPS/Default/Widgets/EndGameScreenWidget.h"
#include "CTF_EndGameScreenWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UCTF_EndGameScreenWidget : public UEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UCTF_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
