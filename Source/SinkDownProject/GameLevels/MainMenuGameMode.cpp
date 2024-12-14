#include "SinkDownProject/GameLevels/MainMenuGameMode.h"
#include "SinkDownProject/UI/MainMenuWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();

            if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
            {
                PC->SetShowMouseCursor(true);
                FInputModeUIOnly InputMode;
                PC->SetInputMode(InputMode);
            }
        }
    }
}
