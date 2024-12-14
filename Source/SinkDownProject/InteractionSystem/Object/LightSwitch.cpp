#include "SinkDownProject/InteractionSystem/Object/LightSwitch.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

ALightSwitch::ALightSwitch()
    : bIsOn(false)
{
    LightSwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Switch Mesh"));
    LightSwitchMesh->SetupAttachment(RootComponent);

    Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
    Light->SetupAttachment(RootComponent);
}

void ALightSwitch::BeginPlay()
{
    Super::BeginPlay();

    Light->SetIntensity(0);
}

void ALightSwitch::OnInteract()
{
    if (bIsOn)
    {
        Light->SetIntensity(0);
        bIsOn = false;
    }
    else
    {
        Light->SetIntensity(1000);
        bIsOn = true;
    }
}