#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SinkDownProject/InteractionSystem/InteractionInterface.h"
#include "InteractiveObjectBase.generated.h"

class UWidgetComponent;
class USceneComponent;

UCLASS(Abstract)
class SINKDOWNPROJECT_API AInteractiveObjectBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInteractiveObjectBase();

protected:
	
	virtual void BeginPlay() override;

	// Called when the actor is interacted with - to be implemented by child classes
	virtual void OnInteract() PURE_VIRTUAL(AInteractiveObjectBase::OnInteract, );

public:
	virtual void InteractionWithMe();
	virtual void ShowInteractionWidget();
	virtual void HideInteractionWidget();

protected:
	UPROPERTY(EditAnywhere) USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere) UWidgetComponent* InteractionWidget;
};
