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

    // �ڷ���Ʈ ���� ������Ʈ��
    UPROPERTY(EditAnywhere, Category = "Teleport")
    ATriggerVolume* TriggerVolume;

    UPROPERTY(EditAnywhere, Category = "Teleport")
    APhysicsDoor* DestinationDoor;

    // �÷��̾ Ʈ���ſ� ������ �� ȣ��� �Լ�
    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    // �ڷ���Ʈ ��ġ ������
    UPROPERTY(EditAnywhere, Category = "Teleport")
    FVector TeleportOffset = FVector(0.0f, 0.0f, 100.0f);
};