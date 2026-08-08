// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_ArcheryComponent.h"
#include "SC_ArcheryArts.generated.h"

UENUM(BlueprintType)
enum class ESC_ArcheryArt : uint8
{
	Normal,
	StrongShot,
	RapidFire,
	MultiShot
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCARLETCOMBAT_API USC_ArcheryArts : public USC_ArcheryComponent
{
	GENERATED_BODY()
	
public:

	// Currently selected archery art
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts")
	ESC_ArcheryArt CurrentArcheryArt = ESC_ArcheryArt::Normal;

	// Array of arrow classes used for archery arts
	// Normal art uses the first arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts")
	TArray<TSubclassOf<AActor>> ArrowClasses;

	// Fetches a new arrow actor of the specified class
	// Default implementation: spawning a new actor
	// Useful for implementing prespawned projectile pools
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|ArcheryArts")
	AActor* FetchNewArrowActor(TSubclassOf<AActor> ArrowClass);
	virtual AActor* FetchNewArrowActor_Implementation(TSubclassOf<AActor> ArrowClass);

	// Changes current archery art and arrow classes
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryArts")
	void SelectArcheryArt(ESC_ArcheryArt NewArcheryArt, TArray<TSubclassOf<AActor>> NewArrowClasses);

protected:

	// Bound to OnShot delegate of the parent class
	UFUNCTION()
	void Shoot();


// NORMAL ART
// Average shooting speed, average launch impulse, single arrow, average cooldown
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|Normal")
	float Normal_DrawTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|Normal")
	float Normal_Cooldown = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|Normal")
	float Normal_LaunchImpulse = 5000.f;

protected:
	void Normal_Select();
	void Normal_Shoot();


// STRONG SHOT
// Low shooting speed, high launch impulse, single arrow, long cooldown
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|StrongShot")
	float StrongShot_DrawTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|StrongShot")
	float StrongShot_Cooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|StrongShot")
	float StrongShot_LaunchImpulse = 8000.f;

protected:
	void StrongShot_Select();
	void StrongShot_Shoot();


// RAPID FIRE
// High shooting speed in bursts, low launch impulse, single arrow, short cooldown withing burst, long cooldown after burst
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	int32 RapidFire_MaxBurstSize = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	float RapidFire_First_DrawTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	float RapidFire_InBurst_DrawTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	float RapidFire_InBurst_Cooldown = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	float RapidFire_PostBurst_Cooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|RapidFire")
	float RapidFire_LaunchImpulse = 3000.f;

protected:
	int32 CurrentBurstIndex = 0;

	void RapidFire_Select();
	void RapidFire_Shoot();


// MULTISHOT
// Low shooting speed, normal launch impulse, multiple arrows, long cooldown
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|MultiShot")
	int32 MultiShot_MaxArrows = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|MultiShot")
	float MultiShot_DrawTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|MultiShot")
	float MultiShot_Cooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|ArcheryArts|MultiShot")
	float MultiShot_LaunchImpulse = 4000.f;

protected:
	void MultiShot_Select();
	void MultiShot_Shoot();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
