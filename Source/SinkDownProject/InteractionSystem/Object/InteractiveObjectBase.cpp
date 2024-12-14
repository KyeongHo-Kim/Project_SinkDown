#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "Components/WidgetComponent.h"

AInteractiveObjectBase::AInteractiveObjectBase()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootSceneComponent;

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidget->SetupAttachment(RootComponent);
}

void AInteractiveObjectBase::BeginPlay()
{
    Super::BeginPlay();
    InteractionWidget->SetVisibility(false);
}

void AInteractiveObjectBase::InteractionWithMe()
{
    OnInteract();
}

void AInteractiveObjectBase::ShowInteractionWidget()
{
    InteractionWidget->SetVisibility(true);
}

void AInteractiveObjectBase::HideInteractionWidget()
{
    InteractionWidget->SetVisibility(false);
}