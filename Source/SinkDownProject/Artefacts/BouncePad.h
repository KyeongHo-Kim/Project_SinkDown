#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncePad.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USceneComponent;
class USoundCue;

UCLASS()
class SINKDOWNPROJECT_API ABouncePad : public AActor
{
    GENERATED_BODY()

public:
    ABouncePad();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BouncePadMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UBoxComponent* TriggerBox;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActer, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    // Box Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounce")
    FVector BoxExtent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounce")
    float BounceStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounce")
    USoundCue* BounceSoundCue;
};