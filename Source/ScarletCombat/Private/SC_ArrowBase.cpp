// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_ArrowBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASC_ArrowBase::ASC_ArrowBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

// Returns a normalized air turbulence vector in the specified location
FVector ASC_ArrowBase::GetTurbulenceVector(const FVector& Location)
{
	FVector SampleCoord = Location * TurbulenceScale;
	float X = 2.f * sin(0.5f * SampleCoord.X) + sin(SampleCoord.X) + 
		0.5f * sin(3 * SampleCoord.X) + 0.3f * sin(5 * SampleCoord.X);
	float Y = 2.f * sin(0.3f * SampleCoord.Y + 3.f) + sin(1.1f * SampleCoord.Y ) + 
		0.5f * sin(3.34f * SampleCoord.Y + 6.f) + 0.3f * sin(6.f * SampleCoord.Y + 18);
	float Z = 2.f * sin(0.4 * SampleCoord.Z + 12.f) + sin(0.9f * SampleCoord.Z + 5.34f) + 
		0.5f * sin(4.34f * SampleCoord.Z - 9.f) + 0.15f * sin(11.5f * SampleCoord.Z + 24);
	return FVector(X, Y, Z).GetSafeNormal();
}

// Called when the game starts or when spawned
void ASC_ArrowBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASC_ArrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGravity(DeltaTime);
	UpdateAirFriction(DeltaTime);
	UpdateLift(DeltaTime);
	UpdateWind(DeltaTime);
	UpdateTurbulence(DeltaTime);
}

// Launches the arrow in the specified direction
void ASC_ArrowBase::Initialize(FVector LaunchDirection, float InitialSpeed)
{
	ProjectileMovement->Velocity = LaunchDirection * InitialSpeed;
}


// Applies gravity force to the arrow
void ASC_ArrowBase::UpdateGravity(float DeltaTime)
{
	FVector Force = GravityDirection * Mass * g * GravityScaleModifier;

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Applies air friction force to the arrow
void ASC_ArrowBase::UpdateAirFriction(float DeltaTime)
{
	float Speed = ProjectileMovement->Velocity.Length();
	FVector ForceDirection = -1.f * ProjectileMovement->Velocity / Speed;
	FVector Force = ForceDirection * 0.5 * AirDensity * Speed * Speed * AirFriction;

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Applies lift force to the arrow
void ASC_ArrowBase::UpdateLift(float DeltaTime)
{
	FVector HorizontalVelocity = ProjectileMovement->Velocity * FVector(1, 1, 0);
	FVector Force = GetActorUpVector() * Lift * (HorizontalVelocity.Length() / ReferenceSpeed);

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Applies wind force to the arrow
void ASC_ArrowBase::UpdateWind(float DeltaTime)
{
	FVector Force = WindDirection * WindSpeed * AirFriction;

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Applies turbulence force to the arrow
void ASC_ArrowBase::UpdateTurbulence(float DeltaTime)
{
	FVector Force = GetTurbulenceVector(GetActorLocation()) * Turbulence * AirFriction * (1 - Stability);

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}


