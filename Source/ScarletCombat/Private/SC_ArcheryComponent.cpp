// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_ArcheryComponent.h"

// Sets default values for this component's properties
USC_ArcheryComponent::USC_ArcheryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USC_ArcheryComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = ESC_ArcheryState::Default;
}


// Called every frame
void USC_ArcheryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Update(DeltaTime);
}


// Updates archery state and timers
void USC_ArcheryComponent::Update(float DeltaTime)
{
	if (CurrentState == ESC_ArcheryState::Draw)
	{
		CurrentDrawTime += DeltaTime;
		if (CurrentDrawTime >= DrawTime)
			OnFullyDrawn.Broadcast();
	}

	else if (CurrentState == ESC_ArcheryState::Release)
	{
		CurrentCooldownTime += DeltaTime;
		if (CurrentCooldownTime >= Cooldown)
		{
			CurrentState = ESC_ArcheryState::Default;
			OnCooldownOver.Broadcast();
		}
	}

	else if (CurrentState == ESC_ArcheryState::Default)
		if (WantsToDraw)
			DrawBow();
}


// Entering release state (when fully drawn and released)
void USC_ArcheryComponent::ReleaseShot()
{
	CurrentState = ESC_ArcheryState::Release;
	CurrentCooldownTime = 0.0f;

	OnShot.Broadcast();
}


// Safely cancels the shot and returns to the default state
void USC_ArcheryComponent::CancelShot()
{
	CurrentState = ESC_ArcheryState::Default;
	OnCanceled.Broadcast();
}


// Begin drawing the bow
void USC_ArcheryComponent::DrawBow()
{
	WantsToDraw = false;

	if (CurrentState == ESC_ArcheryState::Default)
	{
		CurrentState = ESC_ArcheryState::Draw;
		CurrentDrawTime = 0.0f;
	}

	else if (CurrentState == ESC_ArcheryState::Release)
	{
		WantsToDraw = true;
	}
}

// Release bow's string (shoot if fully drawn, cancel if not)
void USC_ArcheryComponent::ReleaseBow()
{
	if (CurrentState == ESC_ArcheryState::Draw)
	{
		if (IsFullyDrawn())
			ReleaseShot();

		else
			CancelShot();
	}

	WantsToDraw = false;
}

// Cancels bow drawing without shooting
void USC_ArcheryComponent::CancelDraw()
{
	if (CurrentState == ESC_ArcheryState::Draw)
		CancelShot();

	WantsToDraw = false;
}

