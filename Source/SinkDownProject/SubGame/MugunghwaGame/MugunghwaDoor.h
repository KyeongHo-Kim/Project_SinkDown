#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MugunghwaDoor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaDoor : public AActor
{
    GENERATED_BODY()

public:
    AMugunghwaDoor();
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UFUNCTION()
    void OpenDoor();

private:
    UPROPERTY(EditAnywhere)
    USceneComponent* RootScene;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* DoorMesh;

    UPROPERTY()
    FRotator TargetRotation;

    UPROPERTY()
    FRotator InitialRotation;

    UPROPERTY()
    bool bIsOpening;

    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenSpeed = 90.0f;

    UPROPERTY(EditAnywhere, Category = "Door")
    float TargetYaw = -90.0f;
};