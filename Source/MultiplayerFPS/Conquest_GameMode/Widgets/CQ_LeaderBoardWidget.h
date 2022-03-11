#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/LeaderBoardWidget.h"
#include "CQ_LeaderBoardWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UCQ_LeaderBoardWidget : public ULeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCQ_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

};
