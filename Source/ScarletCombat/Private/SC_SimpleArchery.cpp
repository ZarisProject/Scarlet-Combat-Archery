// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_SimpleArchery.h"

// Called every frame
void USC_SimpleArchery::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DrawTime = BowDrawTime;
	Cooldown = BowCooldown;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}