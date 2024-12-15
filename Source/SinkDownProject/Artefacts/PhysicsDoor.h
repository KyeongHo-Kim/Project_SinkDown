#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsDoor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ATriggerVolume;

UCLASS()
class SINKDOWNPROJECT_API APhysicsDoor : public AActor
{
    GENERATED_BODY()

public:
    APhysicsDoor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* DoorFrame;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Door;

    // 텔레포트 관련 컴포넌트들
    UPROPERTY(EditAnywhere, Category = "Teleport")
    ATriggerVolume* TriggerVolume;

    UPROPERTY(EditAnywhere, Category = "Teleport")
    APhysicsDoor* DestinationDoor;

    // 플레이어가 트리거에 들어왔을 때 호출될 함수
    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    // 텔레포트 위치 오프셋
    UPROPERTY(EditAnywhere, Category = "Teleport")
    FVector TeleportOffset = FVector(0.0f, 0.0f, 100.0f);
};