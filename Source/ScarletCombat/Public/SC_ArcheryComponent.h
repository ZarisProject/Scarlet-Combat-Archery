// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SC_ArcheryState.h"
#include "SC_ArcheryInterface.h"
#include "SC_ArcheryComponent.generated.h"


// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullyDrawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownOver);


UCLASS( Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCARLETCOMBAT_API USC_ArcheryComponent : public UActorComponent, public ISC_ArcheryInterface
{
	GENERATED_BODY()

protected:

	// Current archery state
	ESC_ArcheryState CurrentState;

	// State transition parameters
	float DrawTime = 1.f;
	float Cooldown = 1.f;

	// Timer counters
	float CurrentDrawTime = 0.0f;
	float CurrentCooldownTime = 0.0f;

	bool FullyDrawn = false;

	// Input cache
	bool WantsToDraw = false;

public:
	// DELEGATES

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnFullyDrawn OnFullyDrawn;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnShot OnShot;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnCanceled OnCanceled;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnCooldownOver OnCooldownOver;

public:	
	// Sets default values for this component's properties
	USC_ArcheryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Updates archery state and timers
	void Update(float DeltaTime);

	// Entering release state (when fully drawn and released)
	void ReleaseShot();

	// Safely cancels the shot and returns to the default state
	void CancelShot();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// PARAMETER SETTERS
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryComponent")
	void SetDrawTime(float InDrawTime) { DrawTime = InDrawTime; }
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryComponent")
	void SetCooldown(float InCooldown) { Cooldown = InCooldown; }

	// INPUTS

	// Begin drawing the bow
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryComponent")
	void DrawBow();

	// Release bow's string (shoot if fully drawn, cancel if not)
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryComponent")
	void ReleaseBow();

	// Cancels bow drawing without shooting
	UFUNCTION(BlueprintCallable, Category = "ScarletCombat|Archery|ArcheryComponent")
	void CancelDraw();

	// GETTERS

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	float GetCurrentDrawTime() { return CurrentDrawTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	float GetDrawTime() { return DrawTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	bool IsFullyDrawn() { return FullyDrawn; }


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	float GetCurrentCooldownTime() { return CurrentCooldownTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	float GetCooldown() { return Cooldown; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	bool IsOnCooldown() { return Cooldown > CurrentCooldownTime; }


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletCombat|Archery|ArcheryComponent")
	ESC_ArcheryState GetCurrentState() { return CurrentState; }


public:
	// INTERFACE IMPLEMENTATION

	// Returns values relevant to character animation blueprints
	virtual void Archery_GetAnimationData_Implementation(
		ESC_ArcheryState& OutCurrentState,
		float& OutDrawTime,
		float& OutCurrentDrawTime,
		float& OutCooldown,
		float& OutCurrentCooldownTime)
	{
		OutCurrentState = CurrentState;
		OutDrawTime = DrawTime;
		OutCurrentDrawTime = CurrentDrawTime;
		OutCooldown = Cooldown;
		OutCurrentCooldownTime = CurrentCooldownTime;
	}
};
