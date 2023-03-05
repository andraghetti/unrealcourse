// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay() {
  Super::BeginPlay();
  StartLocation = GetActorLocation();
}

float AMovingPlatform::GetDistanceMoved() const {
  return FVector::Distance(StartLocation, GetActorLocation());
}

bool AMovingPlatform::ShouldPlatformTurnAround() const {
  return GetDistanceMoved() > MoveDistance;
}

void AMovingPlatform::MovePlatform(float DeltaTime) {
  if (ShouldPlatformTurnAround()) {
    FVector MovingDirection = PlatformMovingSpeed.GetSafeNormal();
    StartLocation = StartLocation + MoveDistance * MovingDirection;
    SetActorLocation(StartLocation);
    PlatformMovingSpeed = -PlatformMovingSpeed;
  } else {
    FVector NewLocation =
        GetActorLocation() + (PlatformMovingSpeed * DeltaTime);
    SetActorLocation(NewLocation);
  }
}

void AMovingPlatform::RotatePlatform(float DeltaTime) {
  UE_LOG(LogTemp, Display, TEXT("RotatePlatform: %s"), *GetName());
  FRotator NewRotation = PlatformRotationSpeed * DeltaTime;
  AddActorLocalRotation(NewRotation);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  MovePlatform(DeltaTime);
  RotatePlatform(DeltaTime);
}
