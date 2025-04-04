#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SFGameInstanceSubsystem.h"
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

	template<typename T>
    T GetCurrentOption() const;

    void SetCurrentOption(FString NewCurrentOption);
};

template<>
inline FString USelectionWidget::GetCurrentOption<FString>() const
{
    return Options.IsValidIndex(CurrentIndex) ? Options[CurrentIndex] : TEXT("");
}

template<>
inline EGameMode USelectionWidget::GetCurrentOption<EGameMode>() const
{
    const UEnum* EnumPtr = StaticEnum<EGameMode>();
    if (!EnumPtr || !Options.IsValidIndex(CurrentIndex))
    {
        return EGameMode::Cooperative; 
    }

    int64 EnumValue = EnumPtr->GetValueByName(FName(*Options[CurrentIndex]));
    return static_cast<EGameMode>(EnumValue);
}

template<>
inline int32 USelectionWidget::GetCurrentOption<int32>() const
{
    return Options.IsValidIndex(CurrentIndex) ? FCString::Atoi(*Options[CurrentIndex]) : 0;
}

template<>
inline bool USelectionWidget::GetCurrentOption<bool>() const
{
    if (!Options.IsValidIndex(CurrentIndex))
    {
        return false;
    }

    FString Value = Options[CurrentIndex].ToLower();
    return (Value == TEXT("true") || Value == TEXT("on") || Value == TEXT("1"));
}