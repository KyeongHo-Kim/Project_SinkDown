#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MugunghwaSurveillanceLight.generated.h"

class ATriggerVolume;
class UStaticMeshComponent;
class USpotLightComponent;
class UMugunghwaGameComponent;

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaSurveillanceLight : public AActor
{
    GENERATED_BODY()

public:
    AMugunghwaSurveillanceLight();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere) UStaticMeshComponent* SurveillanceLightMesh;

    UPROPERTY(VisibleAnywhere)
    USpotLightComponent* SpotLight;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    ATriggerVolume* SurveillanceVolume;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    float MinRotationSpeed = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    float MaxRotationSpeed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    float RandomDirectionChangeInterval = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Surveillance")
    float LightIntensity = 8000.0f;

    UPROPERTY()
    UMugunghwaGameComponent* MugunghwaGameComponent;

    FRotator TargetRotation;
    float CurrentRotationSpeed;
    FTimerHandle RandomizationTimerHandle;

    UFUNCTION()
    void OnPlayerEnterVolume(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnPlayerExitVolume(AActor* OverlappedActor, AActor* OtherActor);

    void SetRandomRotation();
    void StartSurveillance();
    void StopSurveillance();
    bool IsGameActive() const;
};