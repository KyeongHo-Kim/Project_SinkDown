#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MugunghwaObstacle.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class ATriggerVolume;
class USoundBase;

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaObstacle : public AActor
{
    GENERATED_BODY()

public:
    AMugunghwaObstacle();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere) USceneComponent* SceneRoot;

    UPROPERTY(EditAnywhere, Category = "Aeroplane")
    UStaticMeshComponent* MugunghwaObstacleMesh;

    UPROPERTY(EditAnywhere, Category = "Trigger")
    ATriggerVolume* TriggerVolume;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float FallSpeed = 2000.0f; 

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* ImpactSound;

    UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};