/*  Copyright (C) 2022 Alexander Hadzhiev, Yasen Alcev

   MultiplayerFPS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MultiplayerFPS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MultiplayerFPS.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
