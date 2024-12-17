#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "MatrixWorldActivator.generated.h"

class UStaticMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API AMatrixWorldActivator : public AInteractiveObjectBase
{
	GENERATED_BODY()

public:
	AMatrixWorldActivator();

protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh") UStaticMeshComponent* InteractiveMesh;
};
