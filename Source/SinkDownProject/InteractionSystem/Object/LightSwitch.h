#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "LightSwitch.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;

UCLASS()
class SINKDOWNPROJECT_API ALightSwitch : public AInteractiveObjectBase
{
    GENERATED_BODY()

public:
    ALightSwitch();

protected:
    virtual void BeginPlay() override;
    virtual void OnInteract() override;

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* LightSwitchMesh;

    UPROPERTY(EditAnywhere)
    UPointLightComponent* Light;

    bool bIsOn;
};