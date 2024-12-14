#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollableBall.generated.h"

class UStaticMeshComponent;
class ARollingBallEndPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedEndPoint);

UCLASS()
class SINKDOWNPROJECT_API ARollableBall : public AActor
{
	GENERATED_BODY()

public:
	ARollableBall();
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* BallMesh;
	UPROPERTY() ARollingBallEndPoint* TargetEndPoint;

	UFUNCTION() void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(BlueprintAssignable) FOnReachedEndPoint OnReachedEndPoint;
	void SetTargetEndPoint(ARollingBallEndPoint* EndPoint) { TargetEndPoint = EndPoint; }
};