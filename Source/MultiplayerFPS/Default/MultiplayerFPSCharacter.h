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

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
	class UMultiplayerFPSHealthSystem* HealthSystem;

	UPROPERTY()
	TArray<class AMultiplayerFPSFirearm*> FirearmArray;

	UPROPERTY()
	TArray<bool> CanFireFirearmArray;

	UPROPERTY()
	bool bIsReloading;

	UPROPERTY()
	bool bIsZoomedIn;

	UPROPERTY()
	int32 WeaponInHand;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AMultiplayerFPSFirearm>> FirearmClassArray;

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

	UCameraComponent* GetFirstPersonCameraComponent()
	{
		return FirstPersonCameraComponent;
	}

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

	UFUNCTION()
	virtual void StartFiring();

	UFUNCTION()
	virtual void StopFiring();

	UFUNCTION()
	virtual void SwitchWeapon();

	UFUNCTION()
	virtual void SwitchFireMode();

	UFUNCTION()
	virtual void Reload();

	UFUNCTION()
	virtual void Zoom();

	UFUNCTION()
	virtual void ZoomOut();

	UFUNCTION()
	void SetFOV(float FOV);

	UFUNCTION()
	void SetIsReloading();
};

