#include "SinkDownProject/Items/Weapon/Weapon.h"
#include "SinkDownProject/DataAsset/WeaponData.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	UpdateWeaponMesh();
	SetupCollision();
}

void AWeapon::SetWeaponData(UWeaponData* NewWeaponData)
{
	WeaponData = NewWeaponData;
	UpdateWeaponMesh();
	SetupCollision();
}
void AWeapon::UpdateWeaponMesh()
{
	if (WeaponData && WeaponData->SkeletalMesh)
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->SkeletalMesh);
		WeaponMesh->SetVisibility(true);
	}
	else
	{
		WeaponMesh->SetSkeletalMesh(nullptr);
		WeaponMesh->SetVisibility(false);
	}
}

void AWeapon::SetupCollision()
{
	if (WeaponData)
	{
		switch (WeaponData->WeaponType)
		{
		case EWeaponType::MeleeWeapon:
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
			WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
			break;

		case EWeaponType::RangeWeapon:
		default:
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	}
	else
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::Equip(ACharacter* NewOwner, bool bNewIsActive)
{
	if (NewOwner && WeaponData)
	{
		FName SocketName = bNewIsActive ? WeaponData->ActiveSocketName : WeaponData->InactiveSocketName;
		AttachToComponent(NewOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActiveState(bNewIsActive);
	}
}

void AWeapon::Unequip()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetupCollision();
}

void AWeapon::SetWeaponVisibility(bool bIsBisible)
{
	if (WeaponMesh)
	{
		WeaponMesh->SetVisibility(bIsBisible);
	}
}

void AWeapon::SetActiveState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	// You can add additional logic here if needed, such as changing the weapon's appearance or behavior based on its active state
}

UWeaponData* AWeapon::GetWeaponData() const
{
	return WeaponData;
}

void AWeapon::SetGlidingMeshState(bool bUseGlidingMesh)
{
	if (!WeaponData || !WeaponMesh) return;

	if (bUseGlidingMesh && WeaponData->GlidingSkeletalMesh)
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->GlidingSkeletalMesh);
	}
	else if (WeaponData->SkeletalMesh)
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->SkeletalMesh);
	}
}

