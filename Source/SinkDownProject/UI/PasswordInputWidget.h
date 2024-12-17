#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PasswordInputWidget.generated.h"

class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPasswordResult, bool, bSuccess);

UCLASS()
class SINKDOWNPROJECT_API UPasswordInputWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget)) UEditableTextBox* PasswordInput;

    UFUNCTION()
    void OnPasswordInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
    const FString CorrectPassword = TEXT("Klaus");
    void ValidatePassword(const FString& InputPassword);

public:
    UPROPERTY(BlueprintAssignable)FOnPasswordResult OnPasswordResult;
	
};
