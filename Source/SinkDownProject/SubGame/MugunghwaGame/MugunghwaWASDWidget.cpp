#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaWASDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UMugunghwaWASDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TimeRemainingBar)
    {
        TimeRemainingBar->SetPercent(1.0f);
        TimeRemainingBar->SetFillColorAndOpacity(StartColor);
    }
}

void UMugunghwaWASDWidget::UpdateKeySequence(const TArray<FKey>& KeySequence, int32 CurrentIndex)
{
    if (!KeySequenceText) return;

    FString SequenceStr = TEXT("[ ");

    for (int32 i = 0; i < KeySequence.Num(); i++)
    {
        if (i == CurrentIndex)
        {
            SequenceStr += TEXT("[") + KeySequence[i].ToString() + TEXT("]");
        }
        else
        {
            SequenceStr += KeySequence[i].ToString();
        }
        if (i < KeySequence.Num() - 1)
        {
            SequenceStr += TEXT(" | ");
        }
    }

    SequenceStr += TEXT(" ]");

    KeySequenceText->SetText(FText::FromString(SequenceStr));
}

void UMugunghwaWASDWidget::UpdateTimeRemaining(float RemainingTime)
{
    if (!TimeRemainingBar) return;

    float Progress = RemainingTime / TotalTime;
    TimeRemainingBar->SetPercent(Progress);

    FLinearColor NewColor = FMath::Lerp(EndColor, StartColor, Progress);
    TimeRemainingBar->SetFillColorAndOpacity(NewColor);
}

void UMugunghwaWASDWidget::ShowWidget()
{
    SetVisibility(ESlateVisibility::Visible);
    if (!IsInViewport())
    {
        AddToViewport(100);
    }
}

void UMugunghwaWASDWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
}


