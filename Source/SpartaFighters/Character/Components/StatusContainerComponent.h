#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Stats/FStatusStruct.h"
#include "StatusContainerComponent.generated.h"

class ASFCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTAFIGHTERS_API UStatusContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusContainerComponent();

	/*virtual UStatusContainerComponent* GetStatusContainerComponent() const override;*/

	FStatusStruct GetStruct() const;

	void InitializeMovementProperties(ASFCharacter* SFCharacter);

protected:
	virtual void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FStatusStruct StatusStruct;
	
};
