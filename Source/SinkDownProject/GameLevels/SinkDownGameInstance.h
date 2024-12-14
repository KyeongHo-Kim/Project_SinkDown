#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "SinkDownGameInstance.generated.h"

class ULoadingScreenWidget;

UCLASS()
class SINKDOWNPROJECT_API USinkDownGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    // Start loading the game level
    UFUNCTION(BlueprintCallable)  void LoadGameLevel();

protected:
    // Loading screen widget class
    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<ULoadingScreenWidget> LoadingScreenClass;

    // the current loading screen widget instance
    UPROPERTY() ULoadingScreenWidget* LoadingScreenWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI") float MinimumLoadingScreenTime = 4.2f;

private:
    // Streaming Manager
    FStreamableManager StreamableManager;

    // Track loading progress
    int32 LoadedSteps;
    int32 TotalLoadSteps;

    // Loading functions for each step
    void LoadCharacterContent();
    void LoadRespawnContent();
    void LoadUIContent();
    void LoadGameplayContent();

    // callback called when the asset is finished loading
    void OnAssetLoaded();

    // Called when all loading is complete
    void FinishLoading();


    // Track loading screen timing
    float LoadingStartTime;
    bool bLoadingComplete;

    // Checks if minimum time has elapsed and triggers level change
    void CheckAndFinishLoading();
};
