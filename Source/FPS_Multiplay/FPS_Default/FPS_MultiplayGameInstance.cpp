// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayGameInstance.h"
#include "Core/PlayFabClientAPI.h"
#include "MoviePlayer.h"

UFPS_MultiplayGameInstance::UFPS_MultiplayGameInstance(const FObjectInitializer& ObjectInitializer) { }

void UFPS_MultiplayGameInstance::Init()
{
	Super::Init();

	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UFPS_MultiplayGameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UFPS_MultiplayGameInstance::EndLoadingScreen);

	clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
}

void UFPS_MultiplayGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UFPS_MultiplayGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void UFPS_MultiplayGameInstance::BeginLoadingScreen(const FString& MapName)
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

void UFPS_MultiplayGameInstance::EndLoadingScreen(UWorld* InLoadedWorld) {}

void UFPS_MultiplayGameInstance::Host(const FString& LevelName)
{
	/*
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->ClientTravel(LevelName + "?listen -game -port=7778", TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayGameInstance::Join(const FString& Address) -> PlayerController is not Valid !!!"));
	}
	*/

	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		UGameplayStatics::OpenLevel(World, FName(*(LevelName + "?listen")));
		//World->ServerTravel(LevelName + "?listen -game -port=7778");
		//World->ServerTravel(LevelName + "?listen -port=7778"); //like that we can specify port
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayGameInstance::Host(const FString& LevelName) -> World is not Valid !!!"));
	}
}

void UFPS_MultiplayGameInstance::Join(const FString& Address)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayGameInstance::Join(const FString& Address) -> PlayerController is not Valid !!!"));
	}
}

void UFPS_MultiplayGameInstance::RegisterUser(const FString& Username, const FString& Password, const FString& Email, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FRegisterPlayFabUserRequest req;
	req.TitleId = this->TitleId;
	req.Username = Username;
	req.DisplayName = Username;
	req.Password = Password;
	req.Email = Email;

	PlayerName = Username;
	MainMenu = MainMenuWidget;

	clientAPI->RegisterPlayFabUser(req, PlayFab::UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::RegisterOnSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::OnError));
}

void UFPS_MultiplayGameInstance::LoginUser(const FString& Username, const FString& Password, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FLoginWithPlayFabRequest req;
	req.TitleId = this->TitleId;
	req.Username = Username;
	req.Password = Password;

	PlayerName = Username;
	MainMenu = MainMenuWidget;

	clientAPI->LoginWithPlayFab(req, PlayFab::UPlayFabClientAPI::FLoginWithPlayFabDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::LoginOnSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::OnError));
}

void UFPS_MultiplayGameInstance::RecoverPassword(const FString& Email, UMainMenuWidget* MainMenuWidget)
{
	PlayFab::ClientModels::FSendAccountRecoveryEmailRequest req;
	req.TitleId = this->TitleId;
	req.Email = Email;

	MainMenu = MainMenuWidget;

	clientAPI->SendAccountRecoveryEmail(req, PlayFab::UPlayFabClientAPI::FSendAccountRecoveryEmailDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::OnPasswordRecoverySuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UFPS_MultiplayGameInstance::OnError));
}

void UFPS_MultiplayGameInstance::RegisterOnSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& Result)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, FString::Printf(TEXT("Registration: Successful !")));
	if (MainMenu != nullptr)
	{
		MainMenu->GoToMainMenu();
	}
}

void UFPS_MultiplayGameInstance::LoginOnSuccess(const PlayFab::ClientModels::FLoginResult& Result)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, FString::Printf(TEXT("Login: Successful !")));
	if(MainMenu != nullptr)
	{
		MainMenu->GoToMainMenu();
	}
}

void UFPS_MultiplayGameInstance::OnPasswordRecoverySuccess(const PlayFab::ClientModels::FSendAccountRecoveryEmailResult& Result)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, FString::Printf(TEXT("Password Recovery: successfully sent a recovery email !")));
	if (MainMenu != nullptr)
	{
		MainMenu->GoToLoginMenu();
	}
}

void UFPS_MultiplayGameInstance::OnError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("PlayFab Error:  %s"), *ErrorResult.ErrorMessage));
}
