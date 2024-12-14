#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MugunghwaGameComponent.generated.h"

class AMugunghwaTagger;
class AMugunghwaWatcher;
class UMugunghwaGameEffectsData;
class UMugunghwaWASDWidget;
class ADiary;

UENUM()
enum class EMugunghwaState
{
    Inactive,     
    Counting,      
    Turning,        
    WASDGame,       
    Failed,         
    Completed      
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UMugunghwaGameComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMugunghwaGameComponent();

protected:
	virtual void BeginPlay() override;

private: 

    float CountingDuration;
    float TurningDuration;
    float WASDGameDuration;

    FTimerHandle MugunghwaGameStateTimer;

    // WASD gaming related
    UPROPERTY() TArray<FKey> CurrentKeySequence;
    UPROPERTY() int32 CurrentKeyIndex;

    UPROPERTY(EditDefaultsOnly, Category = "Settings") UMugunghwaGameEffectsData* SoundData;


    UPROPERTY() UMugunghwaWASDWidget* WASDWidget;
    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UMugunghwaWASDWidget> WASDWidgetClass;
    FTimerHandle TimerUpdateHandle;

    void UpdateTimeRemaining();

    UPROPERTY(EditDefaultsOnly, Category = "Rewards") TSubclassOf<ADiary> DiaryClass;

public:	

    UPROPERTY() EMugunghwaState CurrentState;

    UPROPERTY() AMugunghwaTagger* TaggerActor;
    UPROPERTY() AMugunghwaWatcher* WatcherActor;


    UFUNCTION() void StartGame();
    void EndGame(bool bSuccess);

    void StartCounting();
    void StartTurning();
    void StartWASDGame();
    bool ValidateKeyInput(FKey PressedKey);

    void ForceEndGame();

    UFUNCTION() ADiary* SpawnDiary();

    EMugunghwaState GetCurrentState() { return CurrentState; }
};
