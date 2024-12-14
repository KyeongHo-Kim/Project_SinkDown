#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "SinkDownProject/Nodes/BehaviorTree_Node.h"
#include "SinkDownProject/Nodes/Action_Node.h"
#include "SinkDownProject/Nodes/Condition_Node.h"
#include "SinkDownProject/Nodes/Sequence_Node.h"
#include "SinkDownProject/Nodes/Selector_Node.h"
#include "BaseEnemyAIController.generated.h"

class ASinkDownProjectCharacter;
class APatrolPath;
class UCombatComponent;
class UEnemySoundData;

UCLASS()
class SINKDOWNPROJECT_API ABaseEnemyAIController : public AAIController
{
    GENERATED_BODY()
public:
    ABaseEnemyAIController();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY()
    UBehaviorTree_Node* RootNode;

    UPROPERTY(VisibleAnywhere, Category = "Player") ASinkDownProjectCharacter* Player;
    UPROPERTY(VisibleAnywhere, Category = "PatrolPath") UCombatComponent* CombatComponent;
    UPROPERTY(VisibleAnywhere, Category = "PatrolPath") APatrolPath* PatrolPath;

    virtual void InitializeBehaviorTree();
    virtual void ExecuteBehaviorTree();

    void UpdateTimers();

    bool Detected();
    ENodeStatus Chase();
    virtual ENodeStatus Suspicion();
    ENodeStatus Patrol();
    
    //Detected
    virtual bool IsPlayerDetected(float Range);

    //Chase
    virtual bool IsInAttackRange(float AttackRange);

    //Suspicion
    bool IsPlayerLost();

    //Attack
    virtual bool StrikeRaange(float Angle, float Range);

    //Patrol
    void AssignPatrolPath();
    bool CanPatrol();
    void MoveToNextWaypoint();
    bool AtWayPoint();
    AActor* GetCurrentWaypoint();
    void CycleWaypoint();

    //Movement-related
    void LookAtTarget(AActor* Destination, float RotationSpeed);
    void MoveTowardsTarget(AActor* Destination, float MoveSpeed);

    FVector GetPlayerLocation() const;
    bool IsPlayerValid() const;

    //Animation
    bool bIsPlayerDetected;

    bool bIsAttacking;
    virtual bool CanMove() const;

    UPROPERTY(EditAnywhere, Category = "Sound") UEnemySoundData* EnemySoundData;
    bool bWasPlayerPreviouslyDetected;

private:
    float TimeSinceLastSawPlayer;

    int32 CurrentWaypointIndex;
    float TimeSinceArrivedPath;
    float WaypointDelay;

public:
    //Debuging
    void VisualizeDetectionRange(float Range);
    void VisualizeStrikeRange(const FVector& StartLocation, const FVector& ForwardVector, float Angle, float Distance, const FColor& Color, float Duration);
    void DrawDebugTriangle(const FVector& A, const FVector& B, const FVector& C, const FColor& Color, float Duration);

    void UpdatePlayerReference();
};