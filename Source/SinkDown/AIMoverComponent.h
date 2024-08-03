#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIMoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWN_API UAIMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIMoverComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	
	void MoveTo(FVector Destination);
};
