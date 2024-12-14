#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollingBallEndPoint.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USoundBase;

UCLASS()
class SINKDOWNPROJECT_API ARollingBallEndPoint : public AActor
{
    GENERATED_BODY()

public:
    ARollingBallEndPoint();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere) USceneComponent* RootScene;
    UPROPERTY(EditAnywhere) UStaticMeshComponent* MeshComponent;

private:
    UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* DestructionSound;

public:
    void StartPhysicsDestruction();
};