#include "SinkDownProject/Items/Weapon/AcquirableItem_Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "SinkDownProject/Items/Weapon/WeaponManager.h"

AAcquirableItem_Weapon::AAcquirableItem_Weapon()
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
    StaticMeshComponent->SetupAttachment(RootComponent);
}

void AAcquirableItem_Weapon::OnInteract()
{
    APawn* InteractingActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (InteractingActor)
    {
        if (UWeaponManager* WeaponManager = InteractingActor->FindComponentByClass<UWeaponManager>())
        {
            WeaponManager->AddWeapon(WeaponData);
            Destroy();
        }
    }
}