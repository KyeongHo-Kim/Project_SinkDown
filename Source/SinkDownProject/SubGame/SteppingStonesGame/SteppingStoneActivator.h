#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "SteppingStoneActivator.generated.h"

class UStaticMeshComponent;
class USteppingStoneGameComponent;

UCLASS()
class SINKDOWNPROJECT_API ASteppingStoneActivator : public AInteractiveObjectBase
{
	GENERATED_BODY()

public:
	ASteppingStoneActivator();

 protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* InteractiveMesh;
	UPROPERTY(EditAnywhere, Category = "Game Component") USteppingStoneGameComponent* SteppingStoneGameComponent;
};
