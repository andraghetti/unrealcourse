// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UGrabber::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr) {
        FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
        PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
    }
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const {
    FVector Start = GetComponentLocation();
    FVector End = Start + GetForwardVector() * MaxGrabDistance;
    // DrawDebugLine(GetWorld(), Start, End, FColor::Red);
    // DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

    FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
    return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity,
                                            ECC_GameTraceChannel2, Sphere);
}

void UGrabber::Grab() {
    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    FHitResult HitResult;
    bool HasHit = UGrabber::GetGrabbableInReach(HitResult);
    if (PhysicsHandle && HasHit) {
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        HitComponent->SetSimulatePhysics(true);
        HitComponent->WakeAllRigidBodies();
        AActor* HitActor = HitResult.GetActor();
        HitActor->Tags.Add("Grabbed");
        HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        PhysicsHandle->GrabComponentAtLocationWithRotation(
            HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
    }
}

void UGrabber::Release() {
    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
        PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
        PhysicsHandle->ReleaseComponent();
    }
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const {
    UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (Result == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
    }
    return Result;
}