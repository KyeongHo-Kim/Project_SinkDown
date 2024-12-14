#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "MugunghwaActivator.generated.h"

class UStaticMeshComponent;
class UMugunghwaGameComponent;
class AMugunghwaTagger;
class AMugunghwaWatcher;

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaActivator : public AInteractiveObjectBase
{
	GENERATED_BODY()
public:
	AMugunghwaActivator();

protected:
	virtual void OnInteract() override;

private:
	UPROPERTY(EditAnywhere) UStaticMeshComponent* InteractiveMesh;

	UPROPERTY(EditAnywhere, Category = "Actor Reference", meta = (AllowPrivateAccess = "true")) UMugunghwaGameComponent* MugunghwaGameComponent;

	UPROPERTY(EditAnywhere, Category = "Actor Reference", meta = (AllowPrivateAccess = "true")) AMugunghwaTagger* TaggerActor;
	UPROPERTY(EditAnywhere, Category = "Actor Reference", meta = (AllowPrivateAccess = "true")) AMugunghwaWatcher* WatcherActor;

	void GameSetup();
};
