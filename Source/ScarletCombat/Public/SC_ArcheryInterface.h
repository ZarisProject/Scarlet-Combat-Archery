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
class SCARLETCOMBAT_ARCHERY_API ISC_ArcheryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Should be implemented by every actor that can be used as an arrow
	// Specifies initial velocity through launch impulse: velocity = impulse / arrow's mass
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|Arrow")
	void Archery_InitializeArrow(const FVector& LaunchImpulse);
	virtual void Archery_InitializeArrow_Implementation(const FVector& LaunchImpulse) {}


	// Should be implemented by the bow actor or by the entity that own the bow
	// Returns location on which a newly shot arrow will spawn
	// ArrowIndex and ArrowCount are used for multishot archery art
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|ArcherActor")
	FVector Archery_GetArrowLaunchLocation(int32 ArrowIndex = 0, int32 ArrowCount = 1);
	virtual FVector Archery_GetArrowLaunchLocation_Implementation(int32 ArrowIndex = 0, int32 ArrowCount = 1) { return FVector::ZeroVector; }

	// Should be implemented by the bow actor or by the entity that own the bow
	// Returns direction in which a newly shot arrow will be launched
	// ArrowIndex and ArrowCount are used for multishot archery art
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletCombat|Archery|ArcherActor")
	FVector Archery_GetArrowLaunchDirection(int32 ArrowIndex = 0, int32 ArrowCount = 1);
	virtual FVector Archery_GetArrowLaunchDirection_Implementation(int32 ArrowIndex = 0, int32 ArrowCount = 1) { return FVector::ForwardVector; }


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
