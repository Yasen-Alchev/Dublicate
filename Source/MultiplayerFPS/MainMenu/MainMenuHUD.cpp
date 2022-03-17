#include "MainMenuHUD.h"
#include "MainMenuWidget.h"

AMainMenuHUD::AMainMenuHUD() {}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MainMenuWidgetClass))
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (IsValid(MainMenuWidget))
		{
			MainMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidgetClass is not Valid!!!"));
	}
}

void AMainMenuHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

