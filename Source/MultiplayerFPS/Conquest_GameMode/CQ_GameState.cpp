#include "CQ_GameState.h"

#include "CQ_InGameHUD.h"

ACQ_GameState::ACQ_GameState()
{
	ResetStats();
	MaxFlagsToCapture = 1;
	bNetLoadOnClient = true;
	bReplicates = true;
}

