
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundComponent.generated.h"


class UPlayerSoundData;
class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API USoundComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USoundComponent();

    void PlayWalkSound();


protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Sound Data") UPlayerSoundData* SoundData;

    float LastStepTime;
};