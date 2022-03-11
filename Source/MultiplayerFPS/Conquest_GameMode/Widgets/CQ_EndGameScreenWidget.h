#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MultiplayerFPS/Default/Widgets/EndGameScreenWidget.h"
#include "CQ_EndGameScreenWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UCQ_EndGameScreenWidget : public UEndGameScreenWidget
{
	GENERATED_BODY()

public:

	UCQ_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


};
