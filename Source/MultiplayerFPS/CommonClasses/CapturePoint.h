#pragma once

#include "CoreMinimal.h"
#include "Teams.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CapturePoint.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACapturePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACapturePoint();

	UFUNCTION()
	void TimelineProgress(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		FVector FlagBottomLocation = FVector(0, 20, 325);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		FVector FlagTopLocation = FVector(0, 20, 1675);

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FTimeline CurveTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PlatformMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* RodMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* FlagMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USphereComponent* CapturePointDetectionRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamRedSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* TeamBlueSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		UMaterialInterface* NeutralSkin;

	UFUNCTION()
		void ChangeCapturePointColor(UMaterialInterface* Material, bool ChangeFlagColor=false);

	UFUNCTION()
		void  ChangeFlagColor(UMaterialInterface* Material);

	UFUNCTION()
		void ShowFlag(bool bIsVisible);

	UFUNCTION()
		void UpdateObjectiveStats();

	UFUNCTION()
		void MoveFlagUp();

	UFUNCTION()
		void MoveFlagDown();

	UFUNCTION()
		void CheckStatus();

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Replicated)
	TEnumAsByte<ETeams> CapturePointTeam;

	UPROPERTY(Replicated)
	bool bIsCaptured;

private:
	UPROPERTY()
	bool bIsFlagBottomPosition;

	UPROPERTY()
	bool bClaimed;

	UPROPERTY()
	UMaterialInterface* myMaterial;
};
