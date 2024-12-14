#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteppingStoneGameComponent.generated.h"

class ATrueGround;
class AFakeGround;

// Structure to store spawn data for fake grounds
USTRUCT()
struct FFakeGroundData
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;

    UPROPERTY()
    TSubclassOf<AFakeGround> Class;
};

// Component that manages the stepping stone mini-game logic
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SINKDOWNPROJECT_API USteppingStoneGameComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USteppingStoneGameComponent();
    void StartGame();
    void ShowHints();

    UPROPERTY(EditAnywhere, Category = "Game Setup")
    TArray<ATrueGround*> TrueGrounds;

    UPROPERTY(EditAnywhere, Category = "Game Setup")
    TArray<AFakeGround*> FakeGrounds;

private:
    UPROPERTY(EditAnywhere, Category = "Visual Effect")
    float DelayBetweenGlows = 0.2f;

    int32 CurrentGlowIndex = 0;
    FTimerHandle SequentialGlowTimer;

    void ShowNextGlow();

    TArray<FFakeGroundData> FakeGroundSpawnData;
    void SaveFakeGroundLocations();
    void RespawnFakeGrounds();

protected:
    virtual void BeginPlay() override;
};