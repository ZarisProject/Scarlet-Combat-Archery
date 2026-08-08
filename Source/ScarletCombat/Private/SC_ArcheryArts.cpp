// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_ArcheryArts.h"

#include "Kismet/KismetMathLibrary.h"

void USC_ArcheryArts::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate ShootFunctionDelegate;
	ShootFunctionDelegate.BindUFunction(this, "Shoot");
	OnShot.Add(ShootFunctionDelegate);
}

void USC_ArcheryArts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Changes current archery art and arrow classes
void USC_ArcheryArts::SelectArcheryArt(ESC_ArcheryArt NewArcheryArt, TArray<TSubclassOf<AActor>> NewArrowClasses)
{
	CurrentArcheryArt = NewArcheryArt;
	ArrowClasses = NewArrowClasses;

	if (CurrentState == ESC_ArcheryState::Draw)
		WantsToDraw = true;

	CurrentState = ESC_ArcheryState::Default;

	switch (CurrentArcheryArt)
	{
	case ESC_ArcheryArt::Normal:		Normal_Select();		return;
	case ESC_ArcheryArt::StrongShot:	StrongShot_Select();	return;
	case ESC_ArcheryArt::RapidFire:		RapidFire_Select();		return;
	case ESC_ArcheryArt::MultiShot:		MultiShot_Select();		return;

	default: return;
	}
}

// Bound to OnShot delegate of the parent class
void USC_ArcheryArts::Shoot()
{
	switch (CurrentArcheryArt)
	{
	case ESC_ArcheryArt::Normal:		Normal_Shoot();			return;
	case ESC_ArcheryArt::StrongShot:	StrongShot_Shoot();		return;
	case ESC_ArcheryArt::RapidFire:		RapidFire_Shoot();		return;
	case ESC_ArcheryArt::MultiShot:		MultiShot_Shoot();		return;

	default: return;
	}
}

// Fetches a new arrow actor of the specified class
// Default implementation: spawning a new actor
// Useful for implementing prespawned projectile pools
AActor* USC_ArcheryArts::FetchNewArrowActor_Implementation(TSubclassOf<AActor> ArrowClass)
{
	return GetWorld()->SpawnActor<AActor>(ArrowClass);
}


// NORMAL ART

void USC_ArcheryArts::Normal_Select()
{
	SetDrawTime(Normal_DrawTime);
	SetCooldown(Normal_Cooldown);
}

void USC_ArcheryArts::Normal_Shoot()
{
	if (!ArrowClasses.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: no valid arrow class is found"));
		return;
	}

	TSubclassOf<AActor> ArrowClass = ArrowClasses[0];

	if (!GetOwner()->Implements<USC_ArcheryInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: owner actor does not implement archery interface"));
		return;
	}

	FVector ArrowOrigin = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchLocation(GetOwner(), 0, 1);
	FVector LaunchDirection = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchDirection(GetOwner(), 0, 1);

	AActor* Arrow = FetchNewArrowActor(ArrowClass);
	if (!Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: failed to fetch an arrow actor"));
		return;
	}

	Arrow->SetActorLocation(ArrowOrigin);
	Arrow->SetActorRotation(LaunchDirection.Rotation());

	if (Arrow->Implements<USC_ArcheryInterface>())
		ISC_ArcheryInterface::Execute_Archery_InitializeArrow(Arrow, LaunchDirection * Normal_LaunchImpulse);
}


// STRONG SHOT

void USC_ArcheryArts::StrongShot_Select()
{
	SetDrawTime(StrongShot_DrawTime);
	SetCooldown(StrongShot_Cooldown);
}

void USC_ArcheryArts::StrongShot_Shoot()
{
	if (!ArrowClasses.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: no valid arrow class is found"));
		return;
	}

	TSubclassOf<AActor> ArrowClass = ArrowClasses[0];

	if (!GetOwner()->Implements<USC_ArcheryInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: owner actor does not implement archery interface"));
		return;
	}

	FVector ArrowOrigin = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchLocation(GetOwner(), 0, 1);
	FVector LaunchDirection = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchDirection(GetOwner(), 0, 1);

	AActor* Arrow = FetchNewArrowActor(ArrowClass);
	if (!Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: failed to fetch an arrow actor"));
		return;
	}

	Arrow->SetActorLocation(ArrowOrigin);
	Arrow->SetActorRotation(LaunchDirection.Rotation());

	if (Arrow->Implements<USC_ArcheryInterface>())
		ISC_ArcheryInterface::Execute_Archery_InitializeArrow(Arrow, LaunchDirection * StrongShot_LaunchImpulse);
}


// RAPID FIRE

void USC_ArcheryArts::RapidFire_Select()
{
	SetDrawTime(RapidFire_First_DrawTime);
	SetCooldown(RapidFire_InBurst_Cooldown);

	CurrentBurstIndex = 0;
}

void USC_ArcheryArts::RapidFire_Shoot()
{
	if (!ArrowClasses.IsValidIndex(CurrentBurstIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: no valid arrow class is found"));
		return;
	}

	TSubclassOf<AActor> ArrowClass = ArrowClasses[CurrentBurstIndex];

	if (!GetOwner()->Implements<USC_ArcheryInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: owner actor does not implement archery interface"));
		return;
	}

	FVector ArrowOrigin = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchLocation(GetOwner(), 0, 1);
	FVector LaunchDirection = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchDirection(GetOwner(), 0, 1);

	AActor* Arrow = FetchNewArrowActor(ArrowClass);
	if (!Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: failed to fetch an arrow actor"));
		return;
	}

	Arrow->SetActorLocation(ArrowOrigin);
	Arrow->SetActorRotation(LaunchDirection.Rotation());

	if (Arrow->Implements<USC_ArcheryInterface>())
		ISC_ArcheryInterface::Execute_Archery_InitializeArrow(Arrow, LaunchDirection * RapidFire_LaunchImpulse);

	// Incrementing burst index
	CurrentBurstIndex++;

	// Burst continuation
	if (CurrentBurstIndex < RapidFire_MaxBurstSize && ArrowClasses.IsValidIndex(CurrentBurstIndex))
	{
		SetDrawTime(RapidFire_InBurst_DrawTime);
		SetCooldown(RapidFire_InBurst_Cooldown);
	}

	// Burst ending
	else
	{
		SetDrawTime(RapidFire_First_DrawTime);
		SetCooldown(RapidFire_PostBurst_Cooldown);
		CurrentBurstIndex = 0;
	}
}


// MULTISHOT

void USC_ArcheryArts::MultiShot_Select()
{
	SetDrawTime(MultiShot_DrawTime);
	SetCooldown(MultiShot_Cooldown);
}

void USC_ArcheryArts::MultiShot_Shoot()
{
	if (!GetOwner()->Implements<USC_ArcheryInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: owner actor does not implement archery interface"));
		return;
	}

	if (ArrowClasses.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: no valid arrow class is found"));
		return;
	}

	int32 ArrowCount = UKismetMathLibrary::Min(MultiShot_MaxArrows, ArrowClasses.Num());

	for (int32 i = 0; i < ArrowCount; i++)
	{
		TSubclassOf<AActor> ArrowClass = ArrowClasses[i];

		FVector ArrowOrigin = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchLocation(GetOwner(), i, ArrowCount);
		FVector LaunchDirection = ISC_ArcheryInterface::Execute_Archery_GetArrowLaunchDirection(GetOwner(), i, ArrowCount);

		AActor* Arrow = FetchNewArrowActor(ArrowClass);
		if (!Arrow)
		{
			UE_LOG(LogTemp, Warning, TEXT("SC ARCHERY ARTS: failed to fetch an arrow actor"));
			continue;
		}

		Arrow->SetActorLocation(ArrowOrigin);
		Arrow->SetActorRotation(LaunchDirection.Rotation());

		if (Arrow->Implements<USC_ArcheryInterface>())
			ISC_ArcheryInterface::Execute_Archery_InitializeArrow(Arrow, LaunchDirection * Normal_LaunchImpulse);
	}
}
