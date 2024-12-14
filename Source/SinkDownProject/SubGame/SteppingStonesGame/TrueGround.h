#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrueGround.generated.h"

class ASinkDownProjectCharacter;

UCLASS()
class SINKDOWNPROJECT_API ATrueGround : public AActor
{
    GENERATED_BODY()

public:
    ATrueGround();
    virtual void Tick(float DeltaTime) override;
    void ShowHint();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* GroundMesh;

    FTimerHandle HintTimerHandle;
    void EndHintEffect();

    UPROPERTY(EditAnywhere, Category = "Visual Effect")
    UMaterialInterface* GroundMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    float GlowTime;
    bool bIsGlowing;

    UPROPERTY(EditAnywhere, Category = "Visual Effect")
    float GlowDuration;

    UPROPERTY(EditAnywhere, Category = "Visual Effect")
    float MaxGlowIntensity;

    UPROPERTY(EditAnywhere, Category = "Visual Effect")
    FLinearColor GlowColor;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* StepSound;

    FTimerHandle SoundCooldownTimer;
    bool bCanPlaySound;
};
