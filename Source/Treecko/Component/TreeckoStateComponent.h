// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "TreeckoStateComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct TREECKO_API FTreeckoStateTreeActorContext
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

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AController> Controller;

    bool operator==(const FTreeckoStateTreeActorContext& Other) const
    {
        return Owner == Other.Owner && Avatar == Other.Avatar && MeshComponent == Other.MeshComponent &&
            AbilitySystemComponent == Other.AbilitySystemComponent && Controller == Other.Controller;
    }
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTreeckoActorContextUpdatedDelegate, const FTreeckoStateTreeActorContext&,
                                            OldContext);

UCLASS(ClassGroup=(Treecko), meta=(BlueprintSpawnableComponent), Blueprintable)
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
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;

public:
    UPROPERTY(BlueprintReadOnly, Replicated)
    FTreeckoStateTreeActorContext ActorContext;

    UFUNCTION(BlueprintCallable)
    virtual void RequestUpdateActorContext();

    UFUNCTION(BlueprintNativeEvent)
    void UpdateActorContext();

    UFUNCTION(Server, Reliable)
    void Server_UpdateActorContext();

    UPROPERTY(BlueprintAssignable)
    FTreeckoActorContextUpdatedDelegate OnActorContextUpdated;

    UFUNCTION(BlueprintNativeEvent)
    UAbilitySystemComponent* SearchAbilitySystemComponent();

    UFUNCTION(BlueprintNativeEvent)
    AController* SearchController();
};
