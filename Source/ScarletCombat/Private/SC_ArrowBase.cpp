// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_ArrowBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASC_ArrowBase::ASC_ArrowBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
}

// Called when the game starts or when spawned
void ASC_ArrowBase::BeginPlay()
{
	Super::BeginPlay();

	PreviousLocation = GetActorLocation();
}

// Called every frame
void ASC_ArrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LocalTime += DeltaTime;

	if (ProjectileMovement->IsActive())
	{
		UpdateRotation(DeltaTime);

		// Hit Detection
		FHitResult HitResult;
		bool Hit = FlightTrace(HitResult);

		if (Hit)
		{
			// Processing surface interactions
			
			float Depth = CalculatePiercingDepth(HitResult);
			float Speed = ProjectileMovement->Velocity.Length();

			if (Depth < BounceThreshold && Speed >= BounceVelocityRequirement)
				Bounce(HitResult);

			else
			{
				SetActorLocation(HitResult.Location + GetActorForwardVector() * (Depth - ArrowLength / 2.f));
				Land(HitResult);
			}
		}

		// Processing flight physics
		UpdateGravity(DeltaTime);
		UpdateAirFriction(DeltaTime);
		UpdateLift(DeltaTime);
		UpdateWind(DeltaTime);
		UpdateTurbulence(DeltaTime);
	
		PreviousLocation = GetActorLocation();
	}
}

// Launches the arrow in the specified direction
void ASC_ArrowBase::Initialize(FVector LaunchDirection, float InitialSpeed)
{
	ProjectileMovement->Velocity = LaunchDirection * InitialSpeed;
}

// Updates rotation interpolation
void ASC_ArrowBase::UpdateRotation(float DeltaTime)
{
	FRotator TargetRotation = ProjectileMovement->Velocity.Rotation();
	float Speed = RotationInterpolationSpeedMultiplier * (Stability + 0.01f) / GetSurfaceAreaMultiplier(ProjectileMovement->Velocity * -1.f);
	FRotator NewRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, Speed);
	SetActorRotation(NewRotation);
}


// FLIGHT PHYSICS

// Applies gravity force to the arrow
void ASC_ArrowBase::UpdateGravity(float DeltaTime)
{
	FVector Force = GravityDirection * Mass * g * GravityScaleModifier;

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Returns aerodynamic surface area multiplier based on the direction of the force
float ASC_ArrowBase::GetSurfaceAreaMultiplier(const FVector& ForceVector)
{
	float angle = UKismetMathLibrary::Dot_VectorVector(ForceVector.GetSafeNormal(), GetActorForwardVector());
	return UKismetMathLibrary::Lerp(SideSurfaceArea, 1.f, abs(angle));
}

// Applies air friction force to the arrow
void ASC_ArrowBase::UpdateAirFriction(float DeltaTime)
{
	float Speed = ProjectileMovement->Velocity.Length();
	FVector ForceDirection = -1.f * ProjectileMovement->Velocity / Speed;
	FVector Force = ForceDirection * 0.5 * AirDensity * Speed * Speed * AirFriction * GetSurfaceAreaMultiplier(ForceDirection);

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
	FVector Force = WindDirection * WindSpeed * AirFriction * GetSurfaceAreaMultiplier(WindDirection);

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}

// Returns a normalized air turbulence vector in the specified location
FVector ASC_ArrowBase::GetTurbulenceVector(const FVector& Location)
{
	FVector SampleCoord = Location * TurbulenceScale + GetWorld()->GetTimeSeconds() * TurbulenceTimeScale;
	float X = 2.f * sin(0.5f * SampleCoord.X) + sin(SampleCoord.X) +
		0.5f * sin(3 * SampleCoord.X) + 0.3f * sin(5 * SampleCoord.X);
	float Y = 2.f * sin(0.3f * SampleCoord.Y + 3.f) + sin(1.1f * SampleCoord.Y) +
		0.5f * sin(3.34f * SampleCoord.Y + 6.f) + 0.3f * sin(6.f * SampleCoord.Y + 18);
	float Z = 2.f * sin(0.4 * SampleCoord.Z + 12.f) + sin(0.9f * SampleCoord.Z + 5.34f) +
		0.5f * sin(4.34f * SampleCoord.Z - 9.f) + 0.15f * sin(11.5f * SampleCoord.Z + 24);

	FVector TurbulenceVector = FVector(X, Y, Z);

	return TurbulenceVector.GetSafeNormal();
}

// Applies turbulence force to the arrow
void ASC_ArrowBase::UpdateTurbulence(float DeltaTime)
{
	FVector Force = GetTurbulenceVector(GetActorLocation()) * Turbulence * (1 - Stability);
	Force *= UKismetMathLibrary::Lerp(0.5f, 2.f, LocalTime / 5.f);
	Force *= ProjectileMovement->Velocity.Size() / ReferenceSpeed;

	ProjectileMovement->Velocity += Force / Mass * DeltaTime;
}


// HIT DETECTION

// Casts a line trace between previous and current arrow location
bool ASC_ArrowBase::FlightTrace(FHitResult& OutHit)
{
	FVector Start = PreviousLocation;
	FVector End = GetActorLocation();

	FCollisionQueryParams Params = FCollisionQueryParams();
	if (IgnoreOwner)
		Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params);
}


// SURFACE INTERACTION

// Landing the arrow
void ASC_ArrowBase::Land(const FHitResult& Hit)
{
	ProjectileMovement->Deactivate();

	// Attachment
	if (Hit.GetComponent())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
		AttachToComponent(Hit.GetComponent(), AttachmentRules, Hit.BoneName);
	}

	// Interface call

	// Delegates
}

// Bouncing the arrow off the surface
void ASC_ArrowBase::Bounce(const FHitResult& Hit)
{
	float Speed = ProjectileMovement->Velocity.Length();
	FVector VelocityDirection = ProjectileMovement->Velocity / (Speed + 1e-6f);

	// Angle factor calculation
	float Angle = UKismetMathLibrary::Dot_VectorVector(Hit.Normal, VelocityDirection);
	float AngleFactor = UKismetMathLibrary::FClamp(1.f - abs(Angle), 0.1f, 1.f);

	// Restitution
	float Restitution = SampleMaterialRestitution(Hit);

	// Resulting Velocity
	float ResultingSpeed = Speed * AngleFactor * Restitution * BounceVelocityDumping;
	FVector ResultingDirection = UKismetMathLibrary::GetReflectionVector(VelocityDirection, Hit.Normal);

	ProjectileMovement->Velocity = ResultingSpeed * ResultingDirection;

	// Offsetting the arrow from the surface
	FVector OffsetedLocation = Hit.Location + Hit.Normal * 5.f;
	PreviousLocation = OffsetedLocation;
	SetActorLocation(OffsetedLocation);
}

// Returns material density in the hit location
float ASC_ArrowBase::SampleMaterialDensity(const FHitResult& Hit)
{
	return 1.0f;
}

// Returns material's restitution (bounciness) in the hit location
float ASC_ArrowBase::SampleMaterialRestitution(const FHitResult& Hit)
{
	return 1.0f;
}


// Calculating how far the arrow will go into the surface
float ASC_ArrowBase::CalculatePiercingDepth(const FHitResult& Hit)
{
	float Speed = ProjectileMovement->Velocity.Length();
	FVector VelocityDirection = ProjectileMovement->Velocity / (Speed + 1e-6f);

	// Angle factor calculation
	float Angle = UKismetMathLibrary::Dot_VectorVector(-1.f * Hit.Normal, VelocityDirection);
	float AngleFactor = abs(Angle);

	// Speed factor calculation
	float SpeedFactor = PiercingFactor * Speed / ReferenceSpeed;

	// Property factor
	float PropertyFactor = (BluntToPiercingBalance + 1.f) / 2.f;

	// Density
	float Density = SampleMaterialDensity(Hit);

	return AngleFactor * SpeedFactor * PropertyFactor / Density;
}
