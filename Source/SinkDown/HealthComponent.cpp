#include "HealthComponent.h"

UHealthComponent::UHealthComponent() : isDead(false)
{
	PrimaryComponentTick.bCanEverTick = true;


}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHealthComponent::IsDead()
{
	return isDead;
}
