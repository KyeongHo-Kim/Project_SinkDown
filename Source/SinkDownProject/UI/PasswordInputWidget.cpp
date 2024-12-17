#include "SinkDownProject/UI/PasswordInputWidget.h"
#include "Components/EditableTextBox.h"
#include "Framework/Application/SlateApplication.h"

void UPasswordInputWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PasswordInput)
	{
		PasswordInput->OnTextCommitted.AddDynamic(this, &UPasswordInputWidget::OnPasswordInputCommitted);
		PasswordInput->SetKeyboardFocus();  // automatically focus input when widget is created
	}
}

void UPasswordInputWidget::OnPasswordInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)	
	{
		FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::SetDirectly);

		// Set to accept both UI and game inputs
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}

		ValidatePassword(Text.ToString());
	}
}

void UPasswordInputWidget::ValidatePassword(const FString& InputPassword)
{
	bool bIsCorrect = InputPassword.Equals(CorrectPassword, ESearchCase::CaseSensitive);

	if (bIsCorrect)
	{
		UE_LOG(LogTemp, Log, TEXT("Password Success!"));
	}

	OnPasswordResult.Broadcast(bIsCorrect);
}