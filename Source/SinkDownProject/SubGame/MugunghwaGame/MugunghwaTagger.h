#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "MugunghwaTagger.generated.h"

class UStaticMeshComponent;

DECLARE_MULTICAST_DELEGATE(FOnMugunghwaInteractSignature);

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaTagger : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
public:
	AMugunghwaTagger();

protected:
	virtual void BeginPlay() override;
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* TaggerMesh;

    UPROPERTY(EditAnywhere) float TurningSpeed;
    bool bIsTurning;
    FRotator InitialRotation;
    FRotator TargetRotation;
    FTimerHandle RotationTimerHandle;

    UFUNCTION() void UpdateRotation();

    UPROPERTY(EditAnywhere, Category = "Destruction") float ExplosionForce;
    UPROPERTY(EditAnywhere, Category = "Destruction")float ExplosionRadius;

public:
    UFUNCTION() void StartCounting();
    UFUNCTION() void StartTurning();
    UFUNCTION() void ResetRotation();

    FOnMugunghwaInteractSignature OnMugunghwaInteract;

    void StartPhysicsDestruction();
};
