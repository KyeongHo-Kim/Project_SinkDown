#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaActivator.h"
#include "Components/StaticMeshComponent.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaGameComponent.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaTagger.h"
#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaWatcher.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

AMugunghwaActivator::AMugunghwaActivator()
{
	InteractiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mugunghwa Activator Mesh"));
	InteractiveMesh->SetupAttachment(RootComponent);

	MugunghwaGameComponent = CreateDefaultSubobject<UMugunghwaGameComponent>(TEXT("MugunghwaGameComponent"));
}

void AMugunghwaActivator::OnInteract()
{
	if (!MugunghwaGameComponent || !TaggerActor || !WatcherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Required components or actors are missing"));
		return;
	}

	if (!IsValid(TaggerActor) ||
		MugunghwaGameComponent->GetCurrentState() != EMugunghwaState::Inactive)
	{
		return;
	}
	
	GameSetup();
}

void AMugunghwaActivator::GameSetup()
{
	MugunghwaGameComponent->TaggerActor = TaggerActor;
	MugunghwaGameComponent->WatcherActor = WatcherActor;

	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (ASinkDownProjectCharacter* SDPCharacter = Cast<ASinkDownProjectCharacter>(PlayerCharacter))
		{
			SDPCharacter->SetMugunghwaGameComponent(MugunghwaGameComponent);
		}
	}

	MugunghwaGameComponent->StartGame();
}
