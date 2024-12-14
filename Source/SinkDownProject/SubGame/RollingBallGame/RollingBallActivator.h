#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "RollingBallActivator.generated.h"

class UStaticMeshComponent;
class URollingBallGameComponent;
class ARollingBallEndPoint;

UCLASS()
class SINKDOWNPROJECT_API ARollingBallActivator : public AInteractiveObjectBase
{
	GENERATED_BODY()
	
public:
	ARollingBallActivator();

protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* InteractiveMesh;

	UPROPERTY(EditAnywhere, Category = "Game Setup") URollingBallGameComponent* RollingBallGameComponent;
	UPROPERTY(EditAnywhere, Category = "Game Setup") ARollingBallEndPoint* TargetEndPoint;

	void GameSetup();
};
