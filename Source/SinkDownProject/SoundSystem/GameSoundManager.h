#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSoundManager.generated.h"

class UAudioComponent;

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Title,
    InGame,
    Pause,
    GameOver
};

USTRUCT(BlueprintType)
struct FGameStateSound
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* BGM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bLooping = true;
};

UCLASS()
class SINKDOWNPROJECT_API AGameSoundManager : public AActor
{
	GENERATED_BODY()
	

public:
    AGameSoundManager();

protected:
    virtual void BeginPlay() override;

private:
    void FadeToNewBGM(USoundBase* NewBGM, float Volume, float FadeOutDuration = 1.0f, float FadeInDuration = 1.0f);

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    UAudioComponent* BGMComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TMap<EGameState, FGameStateSound> GameStateSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TMap<FName, USoundBase*> ZoneBGMs;

    // Called on game state change
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void ChangeGameState(EGameState NewState);

    // Called when entering a specific zone (only works in-game)
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void EnterZone(FName ZoneName);

    // Play sound effects
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySFX(USoundBase* Sound, float Volume = 1.0f);

    // Save the current game state
    UPROPERTY(BlueprintReadOnly, Category = "Audio")
    EGameState CurrentGameState;

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void ReturnToBaseBGM();
};
