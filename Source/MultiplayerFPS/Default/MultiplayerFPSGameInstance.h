// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayFab.h"
#include "MultiplayerFPS/MainMenu/MainMenuWidget.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "MultiplayerFPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UMultiplayerFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UMultiplayerFPSGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		virtual void Init() override;

	UFUNCTION()
		virtual void Shutdown() override;

	UFUNCTION()
		virtual void StartGameInstance() override;

	UFUNCTION()
		void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
		void EndLoadingScreen(UWorld* InLoadedWorld);

	UFUNCTION()
		void Host(const FString& LevelName);

	UFUNCTION()
		void Join(const FString& Address);

	UFUNCTION()
		void	 RegisterUser(const FString& Username, const FString& Password, const FString& Email, UMainMenuWidget* MainMenuWidget);

	UFUNCTION()
		void LoginUser(const FString& Username, const FString& Password, UMainMenuWidget* MainMenuWidget);

	UFUNCTION()
		void	 RecoverPassword(const FString& Email, UMainMenuWidget* MainMenuWidget);

	void RegisterOnSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& Result);

	void LoginOnSuccess(const PlayFab::ClientModels::FLoginResult& Result);

	void OnPasswordRecoverySuccess(const PlayFab::ClientModels::FSendAccountRecoveryEmailResult& Result);

	void OnError(const PlayFab::FPlayFabCppError& ErrorResult);

	UPROPERTY()
		FString PlayerName;

private:

	UPROPERTY()
		FString TitleId = "D4E09";

	PlayFabClientPtr clientAPI;

	UPROPERTY()
		UMainMenuWidget* MainMenu;
};

