#include "AIMoverComponent.h"

UAIMoverComponent::UAIMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UAIMoverComponent::BeginPlay()
{
	Super::BeginPlay();

}



void UAIMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAIMoverComponent::MoveTo(FVector Destination)
{

}

