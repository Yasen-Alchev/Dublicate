/* Copyright (C) 2022 Alexander Hadzhiev, Yasen Alcev

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

#include "MainMenuGameMode.h"
#include "MainMenuHUD.h"
#include "MainMenuPlayerController.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"


AMainMenuGameMode::AMainMenuGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
	HUDClass = AMainMenuHUD::StaticClass();
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (IsValid(CurrentWidget))
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (IsValid(NewWidgetClass))
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (IsValid(CurrentWidget))
        {
            CurrentWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> CurrentWidget is not Valid !!!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> NewWidgetClass is not Valid !!!"));
    }
}
