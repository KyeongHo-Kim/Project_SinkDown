#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaSurveillanceLight.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerVolume.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaGameComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"


AMugunghwaSurveillanceLight::AMugunghwaSurveillanceLight()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SurveillanceLight"));
	RootComponent = SpotLight;

	SurveillanceLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurveillanceLightMesh"));
	SurveillanceLightMesh->SetupAttachment(RootComponent);

	SpotLight->Intensity = LightIntensity;
	SpotLight->OuterConeAngle = 35.0f;
	SpotLight->SetLightColor(FLinearColor(0.5f, 0.0f, 0.1f));
}

void AMugunghwaSurveillanceLight::BeginPlay()
{
	Super::BeginPlay();
	
	if (SurveillanceVolume)
	{
		SurveillanceVolume->OnActorBeginOverlap.AddDynamic(this, &AMugunghwaSurveillanceLight::OnPlayerEnterVolume);
		SurveillanceVolume->OnActorEndOverlap.AddDynamic(this, &AMugunghwaSurveillanceLight::OnPlayerExitVolume);
	}

	SpotLight->SetVisibility(false);

}

bool AMugunghwaSurveillanceLight::IsGameActive() const
{
	if (!MugunghwaGameComponent) return false;

	EMugunghwaState CurrentState = MugunghwaGameComponent->GetCurrentState();
	return CurrentState != EMugunghwaState::Inactive && CurrentState != EMugunghwaState::Failed;
}


void AMugunghwaSurveillanceLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsGameActive() && SpotLight->IsVisible())
	{
		StopSurveillance();
		return;
	}

	if (SpotLight->IsVisible())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CurrentRotationSpeed);
		SetActorRotation(NewRotation);

		if (FMath::IsNearlyEqual(CurrentRotation.Yaw, TargetRotation.Yaw, 1.0f))
		{
			SetRandomRotation();
		}
	}
}

void AMugunghwaSurveillanceLight::SetRandomRotation()
{
	TargetRotation = GetActorRotation();
	TargetRotation.Yaw = FMath::RandRange(200.0f, 380.0f);
	CurrentRotationSpeed = FMath::RandRange(MinRotationSpeed, MaxRotationSpeed);
}

void AMugunghwaSurveillanceLight::StartSurveillance()
{
	if (!IsGameActive())
		return;

	SpotLight->SetVisibility(true);
	SetRandomRotation();

	GetWorld()->GetTimerManager().SetTimer(
		RandomizationTimerHandle,
		this,
		&AMugunghwaSurveillanceLight::SetRandomRotation,
		RandomDirectionChangeInterval,
		true
	);
}

void AMugunghwaSurveillanceLight::StopSurveillance()
{
	SpotLight->SetVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(RandomizationTimerHandle);
}

void AMugunghwaSurveillanceLight::OnPlayerEnterVolume(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ASinkDownProjectCharacter* SDPCharacter = Cast<ASinkDownProjectCharacter>(OtherActor))
	{
		if (!MugunghwaGameComponent)
		{
			MugunghwaGameComponent = SDPCharacter->GetMugunghwaGameComponent();
		}
		StartSurveillance();
	}
}

void AMugunghwaSurveillanceLight::OnPlayerExitVolume(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor))
	{
		StopSurveillance();
	}
}
