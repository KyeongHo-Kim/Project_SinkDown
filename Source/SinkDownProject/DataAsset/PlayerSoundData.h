#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSoundData.generated.h"

class USoundBase;

UCLASS()
class SINKDOWNPROJECT_API UPlayerSoundData : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    USoundBase* WalkSound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Settings")
    float WalkSoundVolume = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Settings")
    float MinTimeBetweenSteps = 0.3f;
};
