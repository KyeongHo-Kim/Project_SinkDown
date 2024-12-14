#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FakeGround.generated.h"

class ASinkDownProjectCharacter;

UCLASS()
class SINKDOWNPROJECT_API AFakeGround : public AActor
{
    GENERATED_BODY()

public:
    AFakeGround();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* GroundMesh;

    UPROPERTY(EditAnywhere, Category = "Destruction") float DestroyDelay = 1.0f;

    void EnablePhysicsAndFall();
    void DestroyGround();

    bool bIsPhysicsEnabled = false;

public:
    bool IsPhysicsEnabled() const { return bIsPhysicsEnabled; }
};