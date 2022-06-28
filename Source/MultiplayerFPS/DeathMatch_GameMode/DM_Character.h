#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"
#include "DM_Character.generated.h"

UCLASS(config = Game)
class MULTIPLAYERFPS_API ADM_Character : public ACharacter
{
	GENERATED_BODY()

public:

	ADM_Character();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
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


	FORCEINLINE virtual  class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
		virtual void SetOptionsMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void ToggleLeaderBoardVisibility();

	UFUNCTION()
		virtual void ToggleOptionsMenu();

	UPROPERTY()
		FString PlayerName;
};

