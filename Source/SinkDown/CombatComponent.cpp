#include "CombatComponent.h"
#include "HealthComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

// 공격가능한 대상인지 체크
bool UCombatComponent::CanAttack(AActor* TargetActor)
{
	if (!TargetActor) return false;

	UHealthComponent* TargetHealth = Cast<UHealthComponent>(TargetActor->FindComponentByClass(UHealthComponent::StaticClass()));
	return TargetHealth != nullptr && !TargetHealth->IsDead();
}

