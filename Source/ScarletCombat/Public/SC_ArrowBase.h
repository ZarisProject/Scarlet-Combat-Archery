// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SC_ArrowBase.generated.h"

// DELEGAGATES
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContact, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLanded, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBounced, const FHitResult&, Hit, float, Angle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPierced, const FHitResult&, Hit, float, Depth);

// CLASS
UCLASS()
class SCARLETCOMBAT_API ASC_ArrowBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	// Cache
	float LocalTime = 0.0;
	FVector PreviousLocation = FVector(0, 0, 0);

public:	
	// Sets default values for this actor's properties
	ASC_ArrowBase();

	// PROPERTIES
	
	// Mass of the arrow, affects it's gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Properties")
	float Mass = 1.0;

	// Determines air resistance applied to the arrow during flight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float AirFriction = 0.4f;

	// Lift force applied to the arrow when it is traveling at the Reference Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float Lift = 250.0;

	// How much resistance the arrow has to air turbulence (0 - 1 range):
	// 0 - no resistance
	// 1 - complete resistance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float Stability = 0.5f;

	// Side profile surface area relative to the front, used for approximating aerodynamics of the arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float SideSurfaceArea = 5.f;

	// How blunt or "piercy" the arrow is
	// -1 is very blunt
	// 1 is very good at piercing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float BluntToPiercingBalance = 0.f;

	// How long is the arrow mesh (used for piercing assuming that arrow's origin is in the center of mass)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float ArrowLength = 100.f;

	// Reduction of velocity after a perfect bounce
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float BounceVelocityDumping = 0.6f;

	// Minimum angle factor value used for bounce velocity dumping when arrow hits the surface at the right (90) angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float BounceMinimumAngleFactor = 0.1f;

	// Reduction of velocity after piercing a surface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float PiercingVelocityDumping = 0.4f;



	// HIT DETECTION

	// Collision channel used for hit detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Hit Detection")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// Shall hit detection ignore arrow's owner actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Hit Detection")
	bool IgnoreOwner = true;


	// MODIFIERS

	// Adjusts gravity scale of the arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float GravityScaleModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	FVector GravityDirection = FVector(0.f, 0.f, -1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float AirDensity = 1.2e-5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	FVector WindDirection = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float WindSpeed = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float Turbulence = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float TurbulenceScale = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float TurbulenceTimeScale = 10.f;

	// How fast arrow's rotation matches the velocity direction
	// Resulting speed = Stability * (1 / SurfaceAreaMultiplier(Velocity * -1)) * RotationInterpolationSpeedMultiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Modifiers")
	float RotationInterpolationSpeedMultiplier = 5.f;


	// CONSTANTS
	
	// Free fall acceleration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Constants")
	float g = 981.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Constants")
	float ReferenceSpeed = 10000.f;

	// How far into the surface of density 1.f the arrow would go if it was traveling at the Reference Speed and 
	// hit the surface at the right (90 degrees) angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Constants")
	float PiercingFactor = 100.f;

	// Piercing depth below which we assume that the arrow bounces off the surface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Constants")
	float BounceThreshold = 10.f;

	// Minimum speed of the arrow that allow it to bounce (needed to prevent infinite bouncing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Constants")
	float BounceVelocityRequirement = 500.f;


	// DELEGATES

	// Fires when the arrow lands, bounces or pierces something
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnContact OnContact;

	// Fires when the arrow lands
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnLanded OnLanded;

	// Fires when the arrow bounces of a surface
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBounced OnBounced;

	// Fires when the arrow pierces a surface and goes right through
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPierced OnPierced;

protected:

	// FLIGHT PHYSICS

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Updates rotation interpolation
	void UpdateRotation(float DeltaTime);

	// Applies gravity force to the arrow
	void UpdateGravity(float DeltaTime);

	// Returns aerodynamic surface area multiplier based on the direction of the force
	float GetSurfaceAreaMultiplier(const FVector& ForceVector);

	// Applies air friction force to the arrow
	void UpdateAirFriction(float DeltaTime);

	// Applies lift force to the arrow
	void UpdateLift(float DeltaTime);

	// Applies wind force to the arrow
	void UpdateWind(float DeltaTime);

	// Returns a normalized air turbulence vector in the specified location
	FVector GetTurbulenceVector(const FVector& Location);

	// Applies turbulence force to the arrow
	void UpdateTurbulence(float DeltaTime);


	// HIT DETECTION

	// Casts a line trace between previous and current arrow location
	bool FlightTrace(FHitResult& OutHit);

	
	// SURFACE INTERACTION
	
	// Landing the arrow
	void Land(const FHitResult& Hit);

	// Bouncing the arrow off the surface
	void Bounce(const FHitResult& Hit);

	// Attempting to go through the surface
	bool Pierce(const FHitResult& Hit, float PiercingDepth);


	// Returns material's density in the hit location
	float SampleMaterialDensity(const FHitResult& Hit);

	// Returns material's restitution (bounciness) in the hit location
	float SampleMaterialRestitution(const FHitResult& Hit);

	// Calculating how far the arrow will go into the surface
	float CalculatePiercingDepth(const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Launches the arrow in the specified direction
	UFUNCTION(BlueprintCallable, Category="ScarletCombat|Archery|Arrow")
	void Initialize(FVector LaunchDirection, float InitialSpeed);
};
