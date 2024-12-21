#include "SinkDownProject/InteractionSystem/Object/Sora.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimMontage.h"

ASora::ASora()
{
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidget->SetupAttachment(RootComponent);
}


void ASora::BeginPlay()
{
	Super::BeginPlay();
	InteractionWidget->SetVisibility(false);
}

void ASora::InteractionWithMe()
{
    if (InteractionMontage)
    {
        PlayAnimMontage(InteractionMontage);
    }
}

void ASora::ShowInteractionWidget()
{
	InteractionWidget->SetVisibility(true);
}

void ASora::HideInteractionWidget()
{
	InteractionWidget->SetVisibility(false);
}

