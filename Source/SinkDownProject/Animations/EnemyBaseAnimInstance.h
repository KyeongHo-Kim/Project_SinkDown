#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SinkDownProject/DataAsset/EnemyData.h"
#include "EnemyBaseAnimInstance.generated.h"

class ACharacter;
class AMeleeEnemyAIController;
class ARangeEnemyAIController;

UCLASS()
class SINKDOWNPROJECT_API UEnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyBaseAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	bool bIsEnemyTypeInitialized;
	bool bIsMeleeEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat") bool bIsDetected;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement") float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement") bool bIsMoving;

	UPROPERTY() ACharacter* Character;
	UPROPERTY() EEnemyType CurrentEnemyType;
	UPROPERTY() AMeleeEnemyAIController* CachedMeleeController;
	UPROPERTY() ARangeEnemyAIController* CachedRangeController;

public:
	void InitializeEnemyType();
	void UpdateMovementState();
	void UpdateCombatState();

};