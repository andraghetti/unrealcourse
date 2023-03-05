// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class OBSTACLEASSAULT_API AMovingPlatform : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AMovingPlatform();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

private:
  UPROPERTY(EditAnywhere, Category = "Moving Platform")
  FVector PlatformMovingSpeed = FVector(100, 0, 0);

  UPROPERTY(EditAnywhere, Category = "Moving Platform")
  FRotator PlatformRotationSpeed;

  UPROPERTY(EditAnywhere, Category = "Moving Platform")
  double MoveDistance = -1;

  FVector StartLocation;

  virtual float GetDistanceMoved() const;
  virtual bool ShouldPlatformTurnAround() const;

  virtual void MovePlatform(float DeltaTime);
  virtual void RotatePlatform(float DeltaTime);
};