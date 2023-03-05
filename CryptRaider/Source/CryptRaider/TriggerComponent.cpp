// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Logging/LogVerbosity.h"
#include "UObject/NameTypes.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay() {
    Super::BeginPlay();

    UE_LOG(LogTemp, Display, TEXT("TriggerComponent available"));
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* UnlockingActor = GetUnlockingActor();
    if (Mover) {
        if (UnlockingActor) {
            UPrimitiveComponent* Component =
                Cast<UPrimitiveComponent>(UnlockingActor->GetRootComponent());
            if (Component) {
                Component->SetSimulatePhysics(false);
            }
            Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
            Mover->SetShouldMove(true);
            // UE_LOG(LogTemp, Display, TEXT("%s moving!!"),
            //    *Mover->GetOwner()->GetActorNameOrLabel());
        } else {
            Mover->SetShouldMove(false);
            // UE_LOG(LogTemp, Display, TEXT("%s stable."),
            // *Mover->GetOwner()->GetActorNameOrLabel());
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("Mover is Null!!"));
    }
}

AActor* UTriggerComponent::GetUnlockingActor() const {
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* OverlappingActor : Actors) {
        bool IsKeyToUnlock = OverlappingActor->ActorHasTag(UnlockTag);
        bool IsGrabbed = OverlappingActor->ActorHasTag("Grabbed");
        if (IsKeyToUnlock && !IsGrabbed) {
            return OverlappingActor;
        }
    }
    return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover) { Mover = NewMover; }