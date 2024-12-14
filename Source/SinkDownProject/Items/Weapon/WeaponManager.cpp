#include "SinkDownProject/Items/Weapon/WeaponManager.h"
#include "SinkDownProject/DataAsset/WeaponData.h"
#include "SinkDownProject/Items/Weapon/Weapon.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/SkillSystem/SkillBase.h"

UWeaponManager::UWeaponManager()
{
    //ActiveWeaponType = EWeaponType::MeleeWeapon;
    ActiveWeaponType = EWeaponType::None;
}

void UWeaponManager::BeginPlay()
{
    Super::BeginPlay();
}

// Add new weapon data, and use AddUnique to prevent duplicate additions of the same weapon.
void UWeaponManager::AddWeapon(UWeaponData* WeaponData)
{
    if (WeaponData)
    {
        AvailableWeapons.AddUnique(WeaponData);
        EquipWeapon(WeaponData->WeaponType);
    }
}

void UWeaponManager::EquipWeapon(EWeaponType WeaponType)
{
    UWeaponData* WeaponToEquip = nullptr;

    for (UWeaponData* Weapon : AvailableWeapons)
    {
        if (Weapon->WeaponType == WeaponType)
        {
            WeaponToEquip = Weapon;
            break;
        }
    }

    if (WeaponToEquip)
    {
        AWeapon* WeaponActor = nullptr;

        if (EquippedWeapons.Contains(WeaponType))
        {
            WeaponActor = EquippedWeapons[WeaponType];
        }
        else
        {
            WeaponActor = SpawnWeapon(WeaponToEquip);
            if (WeaponActor)
            {
                EquippedWeapons.Add(WeaponType, WeaponActor);
            }
            else
            {
                return;
            }
        }

        WeaponActor->SetWeaponData(WeaponToEquip);
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // Always equip as inactive
            WeaponActor->Equip(OwnerCharacter, false);
        }

        UpdateWeaponsVisibility();
    }
}

AWeapon* UWeaponManager::SpawnWeapon(UWeaponData* WeaponData)
{
    if (WeaponData && GetOwner())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if (NewWeapon)
        {
            NewWeapon->SetWeaponData(WeaponData);
            return NewWeapon;
        }
    }
    return nullptr;
}

void UWeaponManager::SetActiveWeapon(EWeaponType NewWeaponType)
{
    if (EquippedWeapons.Contains(NewWeaponType) && NewWeaponType != ActiveWeaponType)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // Set the previously active weapon to inactive
            if (ActiveWeaponType != EWeaponType::None && EquippedWeapons.Contains(ActiveWeaponType))
            {
                EquippedWeapons[ActiveWeaponType]->Equip(OwnerCharacter, false);
            }

            // Set the new weapon to active
            EquippedWeapons[NewWeaponType]->Equip(OwnerCharacter, true);
        }

        ActiveWeaponType = NewWeaponType;
        UpdateWeaponsVisibility();
    }
}


void UWeaponManager::UpdateWeaponsVisibility()
{
    for (auto& Pair : EquippedWeapons)
    {
        Pair.Value->SetWeaponVisibility(true);
    }
}

void UWeaponManager::DeactivateCurrentWeapon()
{
    if (ActiveWeaponType != EWeaponType::None && EquippedWeapons.Contains(ActiveWeaponType))
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            EquippedWeapons[ActiveWeaponType]->Equip(OwnerCharacter, false);
        }
        ActiveWeaponType = EWeaponType::None;
        UpdateWeaponsVisibility();
    }
}

void UWeaponManager::SwitchWeapon()
{
    TArray<EWeaponType> WeaponTypes;
    EquippedWeapons.GetKeys(WeaponTypes);

    if (WeaponTypes.Num() > 0)
    {
        EWeaponType NewWeaponType;
        if (ActiveWeaponType == EWeaponType::None)
        {
            NewWeaponType = WeaponTypes[0];
        }
        else if (WeaponTypes.Num() == 2)
        {
            NewWeaponType = (ActiveWeaponType == WeaponTypes[0]) ? WeaponTypes[1] : WeaponTypes[0];
        }
        else
        {
            NewWeaponType = WeaponTypes[0];
        }

        SetActiveWeapon(NewWeaponType);
    }
}

TArray<UWeaponData*> UWeaponManager::GetAvailableWeapons() const
{
    return AvailableWeapons;
}

UWeaponData* UWeaponManager::GetCurrentWeapon() const
{
    if (EquippedWeapons.Contains(ActiveWeaponType))
    {
        return EquippedWeapons[ActiveWeaponType]->GetWeaponData();
    }
    return nullptr;
}

bool UWeaponManager::IsWeaponEquipped(EWeaponType WeaponType) const
{
    return EquippedWeapons.Contains(WeaponType);
}

EWeaponType UWeaponManager::GetActiveWeaponType() const
{
    return ActiveWeaponType;
}

FVector UWeaponManager::GetCurrentWeaponLocation() const
{
    if (ActiveWeaponType != EWeaponType::None && EquippedWeapons.Contains(ActiveWeaponType))
    {
        AWeapon* CurrentWeapon = EquippedWeapons[ActiveWeaponType];
        if (CurrentWeapon)
        {
            return CurrentWeapon->GetActorLocation();
        }
    }

    return FVector::ZeroVector;
}


void UWeaponManager::BindToSkill(USkillBase* Skill)
{
    if (Skill)
    {
       Skill->OnSkillStateChanged.AddUObject(this, &UWeaponManager::OnSkillStateChangedHandler);
    }
}

void UWeaponManager::SetCurrentWeaponGlidingState(bool bUseGlidingMesh)
{
    if (ActiveWeaponType != EWeaponType::None && EquippedWeapons.Contains(ActiveWeaponType))
    {
        AWeapon* CurrentWeapon = EquippedWeapons[ActiveWeaponType];
        if (CurrentWeapon)
        {
            CurrentWeapon->SetGlidingMeshState(bUseGlidingMesh);
        }
    }
}

void UWeaponManager::OnSkillStateChangedHandler(bool bActive)
{
    SetCurrentWeaponGlidingState(bActive);
}