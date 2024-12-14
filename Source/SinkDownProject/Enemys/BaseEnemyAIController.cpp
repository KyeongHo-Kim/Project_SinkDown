#include "SinkDownProject/Enemys/BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "SinkDownProject/CombatSystem/CombatComponent.h"
#include "DrawDebugHelpers.h"
#include "SinkDownProject/PatrolPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SinkDownProject/DataAsset/EnemySoundData.h"

ABaseEnemyAIController::ABaseEnemyAIController() 
    : bWasPlayerPreviouslyDetected(false)
    , TimeSinceLastSawPlayer(FLT_MAX)
    , CurrentWaypointIndex(0)
    , TimeSinceArrivedPath(FLT_MAX)
{
    PrimaryActorTick.bCanEverTick = true;

    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void ABaseEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    Player = Cast<ASinkDownProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (InPawn)
    {
        AssignPatrolPath();
        InitializeBehaviorTree();
    }
}

void ABaseEnemyAIController::InitializeBehaviorTree()
{
    RootNode = NewObject<USelector_Node>(this);

    // Combat Sequence
    UCondition_Node* PlayerDetected_Condition = NewObject<UCondition_Node>(this);
    PlayerDetected_Condition->SetCondition(FConditionDelegate::CreateUObject(this, &ABaseEnemyAIController::Detected));

    UAction_Node* Chase_Action = NewObject<UAction_Node>(this);
    Chase_Action->SetAction(FActionDelegate::CreateUObject(this, &ABaseEnemyAIController::Chase));

    USequence_Node* Combat_Sequence = NewObject<USequence_Node>(this);
    Combat_Sequence->AddChild(PlayerDetected_Condition);
    Combat_Sequence->AddChild(Chase_Action);


    //Suspicion Sequence
    UCondition_Node* Suspicion_Condition = NewObject<UCondition_Node>(this);
    Suspicion_Condition->SetCondition(FConditionDelegate::CreateUObject(this, &ABaseEnemyAIController::IsPlayerLost));

    UAction_Node* Suspicion_Action = NewObject<UAction_Node>(this);
    Suspicion_Action->SetAction(FActionDelegate::CreateUObject(this, &ABaseEnemyAIController::Suspicion));

    USequence_Node* Suspicion_Sequence = NewObject<USequence_Node>(this);
    Suspicion_Sequence->AddChild(Suspicion_Condition);
    Suspicion_Sequence->AddChild(Suspicion_Action);


    // Patrol Sequence
    UCondition_Node* Patrol_Condition = NewObject<UCondition_Node>(this);
    Patrol_Condition->SetCondition(FConditionDelegate::CreateUObject(this, &ABaseEnemyAIController::CanPatrol));

    UAction_Node* Patrol_Action = NewObject<UAction_Node>(this);
    Patrol_Action->SetAction(FActionDelegate::CreateUObject(this, &ABaseEnemyAIController::Patrol));

    USequence_Node* Patrol_Sequence = NewObject<USequence_Node>(this);
    Patrol_Sequence->AddChild(Patrol_Condition);
    Patrol_Sequence->AddChild(Patrol_Action);

    RootNode->AddChild(Combat_Sequence);
    RootNode->AddChild(Suspicion_Sequence);
    RootNode->AddChild(Patrol_Sequence);
}

void ABaseEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ExecuteBehaviorTree();

    UpdateTimers();
}

void ABaseEnemyAIController::ExecuteBehaviorTree()
{
    if (RootNode)
    {
        RootNode->Tick();
    }
}

void ABaseEnemyAIController::UpdateTimers()
{
    TimeSinceLastSawPlayer += GetWorld()->GetDeltaSeconds();
    TimeSinceArrivedPath += GetWorld()->GetDeltaSeconds();
}


// Detected Behavior
// -----------------------------------------------------------------------------------------------
bool ABaseEnemyAIController::Detected()
{
    if (CombatComponent && CombatComponent->CanAttack(Player) && IsPlayerDetected(600.0f))
    {
        TimeSinceLastSawPlayer = 0;
        bIsPlayerDetected = true;

        if (!bWasPlayerPreviouslyDetected && EnemySoundData && EnemySoundData->DetectionSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                EnemySoundData->DetectionSound,
                GetPawn()->GetActorLocation(),
                EnemySoundData->DetectionVolume
            );
        }

        bWasPlayerPreviouslyDetected = true;

        return true;
    }
    else
    {
        StopMovement();
        bIsPlayerDetected = false;
        bWasPlayerPreviouslyDetected = false;
        return false;
    }
}

bool ABaseEnemyAIController::IsPlayerDetected(float Range)
{
    if (!Player)  return false;

    FVector ActorLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
    float DistanceToPlayer = FVector::Distance(ActorLocation, Player->GetActorLocation());

    //Debuging
    //VisualizeDetectionRange(Range);

    return DistanceToPlayer <= Range;
}


// Chase Behavior
// -----------------------------------------------------------------------------------------------
bool ABaseEnemyAIController::CanMove() const
{
    return !bIsAttacking;
}

ENodeStatus ABaseEnemyAIController::Chase()
{
    if (!CanMove())
    {
        StopMovement();
        return ENodeStatus::Running;
    }

    if (!IsInAttackRange(100.0f))
    {
        MoveToActor(Player);
        return ENodeStatus::Running;
    }
    else
    {
        StopMovement();
        return ENodeStatus::Success;
    }
}

bool ABaseEnemyAIController::IsInAttackRange(float AttackRange)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || !Player) return false;

    bool bIsWithinAttackRange = FVector::Distance(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) <= AttackRange;

    //DrawDebugSphere(GetWorld(), ControlledPawn->GetActorLocation(), AttackRange, 12, FColor::Red, false, -1.0f, 0, 1.0f);

    return bIsWithinAttackRange;
}


// Attack Behavior
// -----------------------------------------------------------------------------------------------

bool ABaseEnemyAIController::StrikeRaange(float Angle, float Range)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || !Player) return false;

    float HalfHeight = 0.0f;
    if (UCapsuleComponent* CapsuleComp = ControlledPawn->GetComponentByClass<UCapsuleComponent>())
    {
        HalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
    }

    FVector FootLocation = ControlledPawn->GetActorLocation() - FVector(0, 0, HalfHeight);

    FVector PlayerFootLocation = Player->GetActorLocation();
    if (UCapsuleComponent* PlayerCapsuleComp = Player->GetComponentByClass<UCapsuleComponent>())
    {
        PlayerFootLocation.Z -= PlayerCapsuleComp->GetScaledCapsuleHalfHeight();
    }

    FVector DirectionToPlayer = PlayerFootLocation - FootLocation;
    DirectionToPlayer.Normalize();

    FVector ForwardVector = ControlledPawn->GetActorForwardVector();
    float DotProduct = FVector::DotProduct(ForwardVector, DirectionToPlayer);
    float ViewAngle = FMath::Acos(DotProduct) * (180.0f / PI);

    //VisualizeStrikeRange(FootLocation, ForwardVector, Angle, Range, FColor::Green, 0.1f);

    return ViewAngle <= Angle;
}


// Suspicion Behavior
// -----------------------------------------------------------------------------------------------
ENodeStatus ABaseEnemyAIController::Suspicion()
{
    LookAtTarget(Player, 2.0f);
    return ENodeStatus::Running;    
}

bool ABaseEnemyAIController::IsPlayerLost()
{
    if (TimeSinceLastSawPlayer < 2.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// Patrol Behavior
// -----------------------------------------------------------------------------------------------
ENodeStatus ABaseEnemyAIController::Patrol()
{
    MoveToNextWaypoint();
    return ENodeStatus::Running;
}

void ABaseEnemyAIController::AssignPatrolPath()
{
    if (!PatrolPath)
    {
        TArray<AActor*> FoundPatrolPaths;

        //Retrieve all Actors of a specific class from the world and store them in an array
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), FoundPatrolPaths);

        APatrolPath* ClosestPatrolPath = nullptr;
        float ClosestDistance = FLT_MAX;

        for (AActor* Actor : FoundPatrolPaths)
        {
            // Check only unassigned paths
            APatrolPath* PatrolPathCandidate = Cast<APatrolPath>(Actor);
            if (PatrolPathCandidate && !PatrolPathCandidate->bIsAssingned)
            {
                float Distance = FVector::Dist(Actor->GetActorLocation(), GetPawn()->GetActorLocation());
                if (Distance < ClosestDistance)
                {
                    ClosestDistance = Distance;
                    ClosestPatrolPath = Cast<APatrolPath>(Actor);
                }
            }
        }

        PatrolPath = ClosestPatrolPath;
        if (PatrolPath)
        {
            PatrolPath->bIsAssingned = true;
        }
    }
}

bool ABaseEnemyAIController::CanPatrol()
{
    if (!Detected() && !IsPlayerLost())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ABaseEnemyAIController::MoveToNextWaypoint()
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || PatrolPath == nullptr) return; 

    AActor* NextWayPoint = GetCurrentWaypoint();

    if (AtWayPoint())
    {
        TimeSinceArrivedPath = 0;
        CycleWaypoint();

        WaypointDelay = FMath::RandRange(1.0f, 3.0f);
    }

    // If your stay times out, go to the next location
    if (TimeSinceArrivedPath > WaypointDelay)
    {
        MoveTowardsTarget(NextWayPoint, 100.0f);
        LookAtTarget(NextWayPoint, 2.0f);
    }
}

// Returns the location of the destination path.
AActor* ABaseEnemyAIController::GetCurrentWaypoint()
{
    return PatrolPath->GetWaypoint(CurrentWaypointIndex);
}

bool ABaseEnemyAIController::AtWayPoint()
{
    float DistanceToWaypoint = FVector::Distance(GetPawn()->GetActorLocation(), GetCurrentWaypoint()->GetActorLocation());
    return DistanceToWaypoint < 200.0f;
}

// Set the following paths (indexes)
void ABaseEnemyAIController::CycleWaypoint()
{
    CurrentWaypointIndex = PatrolPath->GetNextIndex(CurrentWaypointIndex); 
}


// //Movement-related--------------------------------------------------------
void ABaseEnemyAIController::MoveTowardsTarget(AActor* Destination, float MoveSpeed)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;
    if (ACharacter* EnemyCharacter = Cast<ACharacter>(ControlledPawn))
    {
        if (UCharacterMovementComponent* MovementComp = EnemyCharacter->GetCharacterMovement())
        {
            FVector CurrentLocation = EnemyCharacter->GetActorLocation();
            FVector TargetLocation = Destination->GetActorLocation();
            FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

            MovementComp->Velocity = Direction * MoveSpeed;

            float DistanceToMove = MoveSpeed * GetWorld()->GetDeltaSeconds();
            float DistanceToDestination = FVector::Dist(CurrentLocation, TargetLocation);
            FVector NewLocation;
            if (DistanceToDestination <= DistanceToMove)
            {
                NewLocation = TargetLocation;
            }
            else
            {
                NewLocation = CurrentLocation + Direction * DistanceToMove;
            }
            EnemyCharacter->SetActorLocation(NewLocation);
        }
    }
}

void ABaseEnemyAIController::LookAtTarget(AActor* Destination, float RotationSpeed)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || !Player) return;

    FVector TargetLocation = Destination->GetActorLocation();
    FRotator CurrentRotation = ControlledPawn->GetActorRotation();
    FRotator TargetRotation = (TargetLocation - ControlledPawn->GetActorLocation()).Rotation();

    TargetRotation.Pitch = CurrentRotation.Pitch;

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
    ControlledPawn->SetActorRotation(NewRotation);
}


FVector ABaseEnemyAIController::GetPlayerLocation() const
{
    return Player ? Player->GetActorLocation() : FVector::ZeroVector;
}

bool ABaseEnemyAIController::IsPlayerValid() const
{
    return Player != nullptr;
}

//------------------------------------------------------------------
// Drow Debuging
void ABaseEnemyAIController::VisualizeDetectionRange(float Range)
{
    DrawDebugCylinder(GetWorld(), GetPawn()->GetActorLocation() - FVector(0, 0, 50.f), GetPawn()->GetActorLocation() + FVector(0, 0, 50.f), Range, 12, FColor::Blue, false, 0, 0, 1.0f);
}

void ABaseEnemyAIController::VisualizeStrikeRange(const FVector& StartLocation, const FVector& ForwardVector, float Angle, float Distance, const FColor& Color, float Duration)
{
    const FVector UpVector = FVector(0, 0, 1);
    FVector LeftBoundary = StartLocation + ForwardVector.RotateAngleAxis(Angle, UpVector) * Distance;
    FVector RightBoundary = StartLocation + ForwardVector.RotateAngleAxis(-Angle, UpVector) * Distance;

    DrawDebugTriangle(StartLocation, LeftBoundary, RightBoundary, Color, Duration);
}

void ABaseEnemyAIController::DrawDebugTriangle(const FVector& A, const FVector& B, const FVector& C, const FColor& Color, float Duration)
{
    DrawDebugLine(GetWorld(), A, B, Color, false, Duration, 0, 2.0f);
    DrawDebugLine(GetWorld(), B, C, Color, false, Duration, 0, 2.0f);
    DrawDebugLine(GetWorld(), C, A, Color, false, Duration, 0, 2.0f);
}
