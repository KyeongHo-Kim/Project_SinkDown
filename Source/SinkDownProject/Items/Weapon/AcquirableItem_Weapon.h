#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "AcquirableItem_Weapon.generated.h"

class UStaticMeshComponent;
class UWeaponData;

UCLASS()
class SINKDOWNPROJECT_API AAcquirableItem_Weapon : public AInteractiveObjectBase
{
    GENERATED_BODY()

public:
    AAcquirableItem_Weapon();

protected:
    virtual void OnInteract() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UWeaponData* WeaponData;

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StaticMeshComponent;
};