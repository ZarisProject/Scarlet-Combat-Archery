// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SC_ArrowBase.generated.h"

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
	float Lift = 100.0;

	// How much resistance the arrow has to air turbulence (0 - 1 range):
	// 0 - no resistance
	// 1 - complete resistance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Properties")
	float Stability = 0.5f;


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

	// CONSTANTS
	
	// Free fall acceleration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Constants")
	float g = 981.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Constants")
	float ReferenceSpeed = 10000.f;

protected:

	// FLIGHT PHYSICS

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Applies gravity force to the arrow
	void UpdateGravity(float DeltaTime);

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Launches the arrow in the specified direction
	UFUNCTION(BlueprintCallable, Category="ScarletCombat|Archery|Arrow")
	void Initialize(FVector LaunchDirection, float InitialSpeed);
};
