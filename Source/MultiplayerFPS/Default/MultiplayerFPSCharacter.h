#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPSCharacter.generated.h"

UCLASS(config = Game)
class AMultiplayerFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMultiplayerFPSCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	virtual void TurnAtRate(float Rate);

	virtual void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable)
		virtual void SprintStart();

	UFUNCTION(BlueprintCallable)
		virtual void SprintStop();

	UPROPERTY(Replicated)
		bool bIsSprinting;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool bDead;

	UPROPERTY()
		bool bIsInOptionsMenu;

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE virtual  class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
		TEnumAsByte<ETeams> Team;

	UFUNCTION()
		virtual void SetOptionsMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void ToggleLeaderBoardVisibility();

	UFUNCTION()
		virtual TEnumAsByte<ETeams> getTeam() { return Team; }

	UFUNCTION()
		virtual void ToggleOptionsMenu();

	UFUNCTION()
		virtual void InitTeam();

	UPROPERTY()
		FString PlayerName;
};

