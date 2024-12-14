#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class SINKDOWNPROJECT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPath();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


public:

	//Called when an object is created in editor mode
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	bool bIsAssingned;

	int32 GetNextIndex(int32 Index);
	AActor* GetWaypoint(int32 Index);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPath", meta = (AllowPrivateAccess = "true")) TArray<AActor*> Waypoints;

	void DrawPatrolPath();
	void DrawDebugPatrolPath();
};
