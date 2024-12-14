#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurveillanceLight.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;

USTRUCT()
struct FPatrolPoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FRotator Rotation;

    UPROPERTY(EditAnywhere)
    float WaitTime;
};

UCLASS()
class SINKDOWNPROJECT_API ASurveillanceLight : public AActor
{
    GENERATED_BODY()

public:
    ASurveillanceLight();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    USpotLightComponent* SpotLight;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* SurveillanceLightMesh;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    float RotationSpeed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    TArray<FPatrolPoint> PatrolPoints;

private:
    void MoveToNextPatrolPoint();
    void UpdateLightRotation(float DeltaTime);

    int32 CurrentPatrolIndex;
    FRotator TargetRotation;
    FTimerHandle PatrolTimerHandle;
};