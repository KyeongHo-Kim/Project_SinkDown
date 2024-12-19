#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MatrixWorldGameComponent.generated.h"

class AMatrixWorldEnemy;
class AMatrixWorldEnemyWayPoint;

UENUM()
enum class EMatrixWorldState
{
	Inactive,
	Playing,
	Completed,
	Failed
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UMatrixWorldGameComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMatrixWorldGameComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() AMatrixWorldEnemy* TargetEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings") TSubclassOf<AMatrixWorldEnemy> EnemyClass;
	UPROPERTY() TArray<AMatrixWorldEnemyWayPoint*> PathPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Game Settings") float GameDuration;
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings") float InitialEnemySpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings") float SpeedIncreaseRate;

	float RemainingTime;
	float CurrentEnemySpeed;
	FTimerHandle GameTimerHandle;
	EMatrixWorldState CurrentState;


	void UpdateGame();
	void SpawnEnemy();
	void UpdateEnemySpeed();
	void CleanupGame();

public:
	UFUNCTION() void StartGame();
	void EndGame();
	void SetPathPoints(const TArray<AMatrixWorldEnemyWayPoint*>& Points) { PathPoints = Points; }
	EMatrixWorldState GetCurrentState() const { return CurrentState; }
};
