#include "SinkDownProject/UI/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "SinkDownProject/SinkDownProjectGameMode.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "SinkDownProject/GameLevels/SinkDownGameInstance.h"


void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
    }

    if (ManualButton)
    {
        ManualButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnManualClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackClicked);

        BackButton->SetVisibility(ESlateVisibility::Hidden);
    }
}


void UMainMenuWidget::OnStartClicked()
{
    if (USinkDownGameInstance* GameInstance = Cast<USinkDownGameInstance>(GetGameInstance()))
    {
        GameInstance->LoadGameLevel();
    }
}

void UMainMenuWidget::OnManualClicked()
{
    if (ManualImageWidget)
    {
        bool bIsVisible = ManualImageWidget->GetVisibility() == ESlateVisibility::Visible;

        if (bIsVisible)
        {
            if (FadeOutAnimation)
            {
                PlayAnimation(FadeOutAnimation);

                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
                    {
                        ManualImageWidget->SetVisibility(ESlateVisibility::Hidden);
                        if (BackButton) BackButton->SetVisibility(ESlateVisibility::Hidden);
                    }, FadeOutAnimation->GetEndTime(), false);
            }
        }
        else
        {
            // When showing a manual, make it visible first, then fade it in with the
            ManualImageWidget->SetVisibility(ESlateVisibility::Visible);
            if (BackButton) BackButton->SetVisibility(ESlateVisibility::Visible);

            if (FadeInAnimation)
            {
                PlayAnimation(FadeInAnimation);
            }
        }
    }
}

void UMainMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnBackClicked()
{
    if (ManualImageWidget && FadeOutAnimation)
    {
        PlayAnimation(FadeOutAnimation);
        // Fully hide when animation ends
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
            {
                ManualImageWidget->SetVisibility(ESlateVisibility::Hidden);
                if (BackButton) BackButton->SetVisibility(ESlateVisibility::Hidden);
            }, FadeOutAnimation->GetEndTime(), false);
    }
}