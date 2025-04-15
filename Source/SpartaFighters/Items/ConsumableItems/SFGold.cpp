#include "Items/ConsumableItems/SFGold.h"
#include "Character/SFCharacter.h"
#include "Framework/SFPlayerState.h"

USFGold::USFGold()
{
    ItemName = "Gold";
    ItemDescription = FText::FromString(TEXT("Obtain gold."));
    ObtainedGold = 100.0f;
}

void USFGold::Server_ApplyConsumableEffect_Implementation(ASFCharacter* InPlayerCharacter)
{
    Super::Server_ApplyConsumableEffect_Implementation(InPlayerCharacter);

    if (InPlayerCharacter && InPlayerCharacter->GetController())
    {
        ASFCharacter* SFCharacter = Cast<ASFCharacter>(InPlayerCharacter);
        if (SFCharacter && SFCharacter->GetController())
        {
            ASFPlayerState* PS = SFCharacter->GetController()->GetPlayerState<ASFPlayerState>();
            if (PS)
            {
                PS->AddGold(ObtainedGold);
                UE_LOG(LogTemp, Warning, TEXT("%s obtained %f gold. Current gold: %f"), *InPlayerCharacter->GetName(), ObtainedGold, PS->CurrentGold);  
            }
        }
    }
}