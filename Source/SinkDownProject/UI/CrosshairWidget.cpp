#include "SinkDownProject/UI/CrosshairWidget.h"
#include "Components/Image.h"

void UCrosshairWidget::UpdateCrosshairSpread(float SpreadFactor)
{
    if (CrosshairTop)
        CrosshairTop->SetRenderTranslation(FVector2D(0.f, 16.0f * SpreadFactor));
    if (CrosshairBottom)
        CrosshairBottom->SetRenderTranslation(FVector2D(0.f, -16.0f * SpreadFactor));
    if (CrosshairLeft)
        CrosshairLeft->SetRenderTranslation(FVector2D(16.f * SpreadFactor, 0.f));
    if (CrosshairRight)
        CrosshairRight->SetRenderTranslation(FVector2D(-16.f * SpreadFactor, 0.f));
}
