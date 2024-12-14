#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RollingBallGameComponent.generated.h"

class ARollableBall;
class ARollingBallEndPoint;
class ADiary;

UENUM()
enum class ERollingBallState
{
	Inactive,
	Playing,
	Completed,
	Failed
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API URollingBallGameComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URollingBallGameComponent();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY() ERollingBallState CurrentState;

    UPROPERTY(EditDefaultsOnly, Category = "Game Setup") TSubclassOf<ARollableBall> BallClass;

    UPROPERTY() ARollableBall* CurrentBall;
    UPROPERTY() ARollingBallEndPoint* TargetEndPoint;

    UPROPERTY(EditAnywhere, Category = "Game Setup") FVector BallSpawnOffset = FVector(50.0f, 0.0f, 100.f);

    UFUNCTION() void OnBallReachedEndPoint();


    UPROPERTY(EditDefaultsOnly, Category = "Rewards") TSubclassOf<ADiary> DiaryClass;
    UFUNCTION() ADiary* SpawnDiary(const FVector& Location);
public:
    void StartGame();
    void EndGame(bool bSuccess);

    ERollingBallState GetCurrentState() { return CurrentState; }
    void SetTargetEndPoint(ARollingBallEndPoint* EndPoint) { TargetEndPoint = EndPoint; }
};
