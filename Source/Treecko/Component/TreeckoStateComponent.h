// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "TreeckoStateComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct TREECKO_API FTreeckoStateComponentContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> Owner;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> Avatar;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<USkeletalMeshComponent> MeshComponent;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTreeckoActorContextUpdatedDelegate);

UCLASS(ClassGroup=(Treecko), meta=(BlueprintSpawnableComponent))
class TREECKO_API UTreeckoStateComponent : public UStateTreeComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTreeckoStateComponent();
    virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;

    virtual void BeginPlay() override;
    virtual void InitializeComponent() override;
    virtual void StartLogic() override;
    virtual void BeginDestroy() override;

protected:
    virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;

public:
    UPROPERTY(BlueprintReadOnly)
    FTreeckoStateComponentContext ActorContext;

    UFUNCTION(BlueprintCallable)
    void UpdateActorContext();

    UPROPERTY(BlueprintAssignable)
    FTreeckoActorContextUpdatedDelegate OnActorContextUpdated;

    virtual UAbilitySystemComponent* SearchAbilitySystemComponent();
};
