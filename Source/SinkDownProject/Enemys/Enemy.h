#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/CombatSystem/TeamInterface.h"
#include "Enemy.generated.h"

class ASinkDownProjectCharacter;
class UEnemyData;
class UHealthComponent;
class UCombatComponent;

UCLASS()
class SINKDOWNPROJECT_API AEnemy : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Team") ETeam CurrentTeam = ETeam::Enemy;
	UPROPERTY(EditAnywhere, Category = "Enemy") UEnemyData* EnemyData;
	UPROPERTY(VisibleAnywhere, Category = "Player") ASinkDownProjectCharacter* Player;
	UPROPERTY(VisibleAnywhere, Category = "Health") UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, Category = "Combat") UCombatComponent* CombatComponent;

	UFUNCTION() void OnEnemyDeath();

	virtual void Attack();
	virtual float GetAttackRange();
	virtual float GetDetectedRange();
	FVector GetAttackOrigin() const;

	virtual void PlayAttackMontage() { }
	virtual float PlayDeathMontage() { return 3.0f; }

public:
	virtual ETeam GetTeam() const override { return CurrentTeam; }
	virtual void SetTeam(ETeam NewTeam) override { CurrentTeam = NewTeam; }
};
