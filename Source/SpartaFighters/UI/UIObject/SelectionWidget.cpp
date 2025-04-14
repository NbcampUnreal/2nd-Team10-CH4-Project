#include "UI/UIObject/SelectionWidget.h"
#include "SelectionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

   /* if (LeftArrowButton)
    {
        LeftArrowButton->OnClicked.AddUniqueDynamic(this, &USelectionWidget::OnLeftArrowClicked);
    }

    if (RightArrowButton)
    {
        RightArrowButton->OnClicked.AddUniqueDynamic(this, &USelectionWidget::OnRightArrowClicked);
    }*/
}

void USelectionWidget::InitializeSelection(const TArray<FString>& InOptions, int32 DefaultIndex)
{
    Options = InOptions;
    CurrentIndex = FMath::Clamp(DefaultIndex, 0, Options.Num() - 1);

    if (SelectionText)
    {
        SelectionText->SetText(FText::FromString(Options[CurrentIndex]));
    }
}

//void USelectionWidget::OnLeftArrowClicked()
//{
//    if (Options.Num() == 0)
//    {
//        return;
//    }
//
//    CurrentIndex = (CurrentIndex - 1 + Options.Num()) % Options.Num();
//    SelectionText->SetText(FText::FromString(Options[CurrentIndex]));
//
//    OnSelectionChanged.Broadcast(CurrentIndex);
//}
//
//void USelectionWidget::OnRightArrowClicked()
//{
//    if (Options.Num() == 0)
//    {
//        return;
//    }
//    
//    CurrentIndex = (CurrentIndex + 1) % Options.Num();
//    SelectionText->SetText(FText::FromString(Options[CurrentIndex]));
//
//    OnSelectionChanged.Broadcast(CurrentIndex);
//}

void USelectionWidget::SetCurrentOption(FString NewCurrentOption)
{
    int32 NewIndex = Options.IndexOfByKey(NewCurrentOption);

    if (NewIndex == CurrentIndex || NewIndex == INDEX_NONE)
    {
        return;
    }

    CurrentIndex = NewIndex;

    if (SelectionText)
    {
        SelectionText->SetText(FText::FromString(Options[CurrentIndex]));
    }

    OnSelectionChanged.Broadcast(CurrentIndex);
}