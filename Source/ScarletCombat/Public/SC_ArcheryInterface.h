// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SC_ArcheryState.h"
#include "SC_ArcheryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USC_ArcheryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCARLETCOMBAT_API ISC_ArcheryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	// Returns physical density of the material in the hit spot
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="ScarletCombat|Archery|PhysicsProperties")
	float Archery_GetMaterialDensity(const FHitResult& Hit);
	virtual float Archery_GetMaterialDensity_Implementation(const FHitResult& Hit) { return 0.0; }

	// Returns physical restitution (bounciness) of the material in the hit spot
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|PhysicsProperties")
	float Archery_GetMaterialRestitution(const FHitResult& Hit);
	virtual float Archery_GetMaterialRestitution_Implementation(const FHitResult& Hit) { return 0.0; }


	// Returns values relevant to character animation blueprints
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|Animation")
	void Archery_GetAnimationData(
		ESC_ArcheryState& OutCurrentState,
		float& OutDrawTime,
		float& OutCurrentDrawTime,
		float& OutCooldown,
		float& OutCurrentCooldownTime);
	virtual void Archery_GetAnimationData_Implementation(
		ESC_ArcheryState& OutCurrentState,
		float& OutDrawTime,
		float& OutCurrentDrawTime,
		float& OutCooldown,
		float& OutCurrentCooldownTime) {}


	// Fires when an arrow makes contact with actor / component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|HitEvents")
	void Archery_OnArrowContact(AActor* Arrow, const FHitResult& Hit);
	virtual void Archery_OnArrowContact_Implementation(AActor* Arrow, const FHitResult& Hit) {}

	// Fires when an arrow lands onto actor / component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|HitEvents")
	void Archery_OnArrowLanded(AActor* Arrow, const FHitResult& Hit);
	virtual void Archery_OnArrowLanded_Implementation(AActor* Arrow, const FHitResult& Hit) {}

	// Fires when an arrow bounces off actor / component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|HitEvents")
	void Archery_OnArrowBounced(AActor* Arrow, const FHitResult& Hit, float Angle);
	virtual void Archery_OnArrowBounced_Implementation(AActor* Arrow, const FHitResult& Hit, float Angle) {}

	// Fires when an arrow pierces through actor / component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|HitEvents")
	void Archery_OnArrowPierced(AActor* Arrow, const FHitResult& Hit, float Depth);
	virtual void Archery_OnArrowPierced_Implementation(AActor* Arrow, const FHitResult& Hit, float Depth) {}
};
