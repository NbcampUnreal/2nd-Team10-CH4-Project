

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DodgeEndNoitfy.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAFIGHTERS_API UDodgeEndNoitfy : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
