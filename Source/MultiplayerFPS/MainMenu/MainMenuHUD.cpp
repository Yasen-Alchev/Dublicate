#include "MainMenuHUD.h"
#include "MainMenuWidget.h"

AMainMenuHUD::AMainMenuHUD() {}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(MainMenuWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidgetClass is not Valid!!!"));
		return;
	}

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

	if (!IsValid(MainMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidget is not Valid!!!"));
		return;
	}

	MainMenuWidget->AddToViewport();
}

void AMainMenuHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

