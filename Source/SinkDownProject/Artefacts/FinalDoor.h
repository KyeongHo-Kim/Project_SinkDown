#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalDoor.generated.h"

class USoundBase;

UCLASS()
class SINKDOWNPROJECT_API AFinalDoor : public AActor
{
    GENERATED_BODY()

public:
    AFinalDoor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* DoorMesh;

    UPROPERTY(EditAnywhere, Category = "Door")
    float OpenAngle = 180.0f;

    UPROPERTY(EditAnywhere, Category = "Door|Sound")
    USoundBase* DoorOpenSound;

private:
    UFUNCTION() void OnAllDiaryPiecesCollected();

    void OpenDoor();
};