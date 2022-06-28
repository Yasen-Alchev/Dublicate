#include "CQ_GameState.h"

#include "CQ_InGameHUD.h"
#include "Net/UnrealNetwork.h"

ACQ_GameState::ACQ_GameState()
{
	ResetStats();
	MaxFlagsToCapture = 1;
	bNetLoadOnClient = true;
	bReplicates = true;
}

void ACQ_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACQ_GameState, BlueTeamCapturedFlags);
	DOREPLIFETIME(ACQ_GameState, RedTeamCapturedFlags);
}
