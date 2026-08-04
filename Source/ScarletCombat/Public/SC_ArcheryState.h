#pragma once

#include "CoreMinimal.h"
#include "SC_ArcheryState.generated.h"

UENUM(BlueprintType)
enum class ESC_ArcheryState : uint8
{
	Default,
	Draw,
	Release
};