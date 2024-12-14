#include "SinkDownProject/SoundSystem/GameSoundManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


AGameSoundManager::AGameSoundManager()
{
    BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
    RootComponent = BGMComponent;
}

// Called when the game starts or when spawned
void AGameSoundManager::BeginPlay()
{
	Super::BeginPlay();

    /*ChangeGameState(EGameState::InGame);*/

    FString CurrentLevelName = GetWorld()->GetMapName();

    // Title state if at the main menu level, InGame state otherwise.
    if (CurrentLevelName.Contains(TEXT("MainMenuMap")))
    {
        ChangeGameState(EGameState::Title);
    }
    else
    {
        ChangeGameState(EGameState::InGame);
    }
}

void AGameSoundManager::ChangeGameState(EGameState NewState)
{
    CurrentGameState = NewState;

    if (const FGameStateSound* StateSound = GameStateSounds.Find(NewState))
    {
        if (StateSound->BGM)
        {
            FadeToNewBGM(StateSound->BGM, StateSound->Volume);
        }
    }
}

void AGameSoundManager::EnterZone(FName ZoneName)
{
    // Zone BGM can only be changed while in-game
    if (CurrentGameState == EGameState::InGame)
    {
        if (USoundBase* ZoneBGM = ZoneBGMs.FindRef(ZoneName))
        {
            FadeToNewBGM(ZoneBGM, 1.0f);
        }
    }
}

void AGameSoundManager::PlaySFX(USoundBase* Sound, float Volume)
{
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(this, Sound, Volume);
    }
}

void AGameSoundManager::FadeToNewBGM(USoundBase* NewBGM, float Volume, float FadeOutDuration, float FadeInDuration)
{
    if (!NewBGM || !BGMComponent) return;  // BGMComponent 체크 추가

    // Fade out if the current BGM is playing
    if (BGMComponent->IsPlaying())
    {
        BGMComponent->FadeOut(FadeOutDuration, 0.0f);
    }

    //  Capture this by weak reference
    TWeakObjectPtr<AGameSoundManager> WeakThis(this);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis, NewBGM, Volume, FadeInDuration]()
        {
            if (WeakThis.IsValid())  // check if this is still valid
            {
                if (WeakThis->BGMComponent)  // Check if BGMComponent is valid
                {
                    WeakThis->BGMComponent->SetSound(NewBGM);
                    WeakThis->BGMComponent->Play();
                    WeakThis->BGMComponent->FadeIn(FadeInDuration, Volume);
                }
            }
        }, FadeOutDuration, false);
}

void AGameSoundManager::ReturnToBaseBGM()
{
    if (CurrentGameState == EGameState::InGame)
    {
        // Return to the default BGM in the InGame state
        if (const FGameStateSound* StateSound = GameStateSounds.Find(EGameState::InGame))
        {
            if (StateSound->BGM)
            {
                FadeToNewBGM(StateSound->BGM, StateSound->Volume);
            }
        }
    }
}