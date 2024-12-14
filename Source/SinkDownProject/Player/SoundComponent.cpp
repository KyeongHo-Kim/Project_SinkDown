#include "SinkDownProject/Player/SoundComponent.h"
#include "SinkDownProject/DataAsset/PlayerSoundData.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundComponent::USoundComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    LastStepTime = 0.0f;
}

void USoundComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USoundComponent::PlayWalkSound()
{
    if (!SoundData || !SoundData->WalkSound) return;

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastStepTime < SoundData->MinTimeBetweenSteps)
    {
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(
        this,
        SoundData->WalkSound,
        GetOwner()->GetActorLocation(),
        SoundData->WalkSoundVolume
    );
    LastStepTime = CurrentTime;
}


