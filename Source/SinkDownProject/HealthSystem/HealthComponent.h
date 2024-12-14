#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageHandlerInterface.h"
#include "HealthComponent.generated.h"

class UEnemyData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamageSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SINKDOWNPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Health") UObject* DamageHandler;
	UPROPERTY(EditAnywhere, Category = "EnemyData") UEnemyData* EnemyData;
    UPROPERTY(EditAnywhere, Category = "Health") int32 MaxHealth;

public:
	UPROPERTY(BlueprintAssignable, Category = "Health") FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable, Category = "Health") FOnTakeDamageSignature OnTakeDamage;

    bool IsDead() const;
    void TakeDamage(bool bIsMelee);
	void TakeMaxDamage();
	
	UFUNCTION() void Reset();
};
