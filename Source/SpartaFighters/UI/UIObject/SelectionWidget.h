#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, int32, NewIndex);

UCLASS()
class SPARTAFIGHTERS_API USelectionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* LeftArrowButton;

    UPROPERTY(meta = (BindWidget))
    UButton* RightArrowButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SelectionText;

    TArray<FString> Options;
    int32 CurrentIndex = 0;

    UFUNCTION()
    void OnLeftArrowClicked();

    UFUNCTION()
    void OnRightArrowClicked();

public:

    UPROPERTY(BlueprintAssignable)
    FOnSelectionChanged OnSelectionChanged;

    void InitializeSelection(const TArray<FString>& InOptions, int32 DefaultIndex = 0);
    FString GetCurrentOption() const { return Options.IsValidIndex(CurrentIndex) ? Options[CurrentIndex] : TEXT(""); }
    void SetCurrentOption(FString NewCurrentOption);
};
