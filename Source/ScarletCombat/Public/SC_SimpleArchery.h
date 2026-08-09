// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_ArcheryComponent.h"
#include "SC_SimpleArchery.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCARLETCOMBAT_ARCHERY_API USC_SimpleArchery : public USC_ArcheryComponent
{
	GENERATED_BODY()
	
public:

	// How long does it take to draw the bow (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float BowDrawTime = 1.f;

	// Cooldown time after a succesful shot (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float BowCooldown = 1.f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
