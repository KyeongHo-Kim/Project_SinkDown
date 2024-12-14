#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{	
	AT_Melee UMETA(DisplayName = "Melee"),
	AT_Range UMETA(DisplayName = "Range")
};

//Declaring an attack delegate
DECLARE_DELEGATE_OneParam(FAttackDelegate, AActor*);
DECLARE_DELEGATE_RetVal(FVector, FGetLocationDelegate);
DECLARE_DELEGATE_RetVal(FVector, FGetForwardVectorDelegate);

class ABaseProjectile;
class UCombatEffectManager;
class UCombatEffectData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
		
public:
	//Delegate Function
	void SetAttackType(EAttackType NewAttackType);
	void SetLocationDelegate(const FGetLocationDelegate& Delegate);
	void SetForwardVectorDelegate(const FGetForwardVectorDelegate& Delegate);

	bool CanAttack(AActor* TargetActor);
	void Attack(AActor* TargetActor);
	void SetWeaponRange(float NewRange) { WeaponRange = NewRange; }

	UFUNCTION() void OnProjectileHit(AActor* HitActor, AActor* ProjectileOwner, FVector HitLocation);

private:
	//Delegate
	EAttackType AttackType;
	FAttackDelegate AttackFunction;
	FGetLocationDelegate GetLocationDelegate;
	FGetForwardVectorDelegate GetForwardVectorDelegate;

	
	UPROPERTY(EditDefaultsOnly, Category = "Combat") TSubclassOf<ABaseProjectile> Projectiles;
	UPROPERTY() ABaseProjectile* Projectile;
	float WeaponRange;
	UPROPERTY() UCombatEffectManager* CombatEffectManager;
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true")) UCombatEffectData* EffectData;

	void MeleeAttack(AActor* TargetActor);
	void RangeAttack(AActor* TargetActor);
	void ApplyDamage(AActor* TargetActor, bool IsMelee);
	void PlayAttackEffects();
};
