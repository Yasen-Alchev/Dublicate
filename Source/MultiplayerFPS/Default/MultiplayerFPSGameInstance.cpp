#include "MultiplayerFPSGameInstance.h"
#include "Core/PlayFabClientAPI.h"
#include "MoviePlayer.h"
#include "MultiplayerFPSPlayerState.h"

UMultiplayerFPSGameInstance::UMultiplayerFPSGameInstance(const FObjectInitializer& ObjectInitializer) { }

void UMultiplayerFPSGameInstance::Init()
{
	Super::Init();

	ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
}

void UMultiplayerFPSGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMultiplayerFPSGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void UMultiplayerFPSGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UMultiplayerFPSGameInstance::EndLoadingScreen(UWorld* InLoadedWorld) {}

void UMultiplayerFPSGameInstance::Host(const FString& LevelName)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSGameInstance::Host(const FString& LevelName) -> World is not Valid !!!"));
		return;
	}

	World->ServerTravel(LevelName + "?listen"); 
}

void UMultiplayerFPSGameInstance::Join(const FString& Address, const FString& Port)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSGameInstance::Join(const FString& Address) -> PlayerController is not Valid !!!"));
		return;
	}
	PlayerController->ClientTravel(Address, TRAVEL_Absolute);
}

void UMultiplayerFPSGameInstance::RegisterUser(const FString& Username, const FString& Password, const FString& Email, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FRegisterPlayFabUserRequest req;
	req.TitleId = this->TitleId;
	req.Username = Username;
	req.DisplayName = Username;
	req.Password = Password;
	req.Email = Email;

	PlayerName = Username;
	MainMenu = MainMenuWidget;

	ClientAPI->RegisterPlayFabUser(req, PlayFab::UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::RegisterOnSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::OnError));
}

void UMultiplayerFPSGameInstance::LoginUser(const FString& Username, const FString& Password, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FLoginWithPlayFabRequest req;
	req.TitleId = this->TitleId;
	req.Username = Username;
	req.Password = Password;

	PlayerName = Username;
	MainMenu = MainMenuWidget;

	ClientAPI->LoginWithPlayFab(req, PlayFab::UPlayFabClientAPI::FLoginWithPlayFabDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::LoginOnSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::OnError));
}

void UMultiplayerFPSGameInstance::RecoverPassword(const FString& Email, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FSendAccountRecoveryEmailRequest req;
	req.TitleId = this->TitleId;
	req.Email = Email;

	MainMenu = MainMenuWidget;

	ClientAPI->SendAccountRecoveryEmail(req, PlayFab::UPlayFabClientAPI::FSendAccountRecoveryEmailDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::OnPasswordRecoverySuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UMultiplayerFPSGameInstance::OnError));
}

void UMultiplayerFPSGameInstance::RegisterOnSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& Result)
{
	if (IsValid(MainMenu))
	{
		MainMenu->GoToMainMenu();
	}
	//AMultiplayerFPSPlayerState* PlayerStateVar = Cast<AMultiplayerFPSPlayerState>();
}

void UMultiplayerFPSGameInstance::LoginOnSuccess(const PlayFab::ClientModels::FLoginResult& Result)
{
	if (IsValid(MainMenu))
	{
		MainMenu->GoToMainMenu();
	}
}

void UMultiplayerFPSGameInstance::OnPasswordRecoverySuccess(const PlayFab::ClientModels::FSendAccountRecoveryEmailResult& Result)
{
	if (IsValid(MainMenu))
	{
		MainMenu->GoToLoginMenu();
	}
}

void UMultiplayerFPSGameInstance::OnError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("PlayFab Error:  %s"), *ErrorResult.ErrorMessage));
}
