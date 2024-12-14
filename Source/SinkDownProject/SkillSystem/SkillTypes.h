#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    None     UMETA(DisplayName = "None"),
    Gliding  UMETA(DisplayName = "Gliding"),
    Parry    UMETA(DisplayName = "Parry"),
};