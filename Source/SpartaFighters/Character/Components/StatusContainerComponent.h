#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Stats/FStatusStruct.h"
#include "StatusContainerComponent.generated.h"

class ASFCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARTAFIGHTERS_API UStatusContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusContainerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	//void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	FStatusStruct StatusStruct;

public:
	void InitStatusComponent(ACharacter* Character);
	void InitializeMovementProperties(ACharacter* Character);
	FStatusStruct GetStruct() const;

	float GetStatusValue(EStatusType Type);

	UFUNCTION()
	void ModifyStatus(EStatusType Type, float Amount);

	void SetHP(float Amount);

	UFUNCTION()
	void OnRep_CurrentHP();
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	int CurrentHP;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	int MaxHP;
	
};
