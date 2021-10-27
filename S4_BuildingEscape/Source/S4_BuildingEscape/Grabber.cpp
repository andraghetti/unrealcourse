// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber ready."));

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not set for: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not set for: %s"), *GetOwner()->GetName());
	}
	else
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("GRAB"));

	FVector LineTraceEnd = GetLineTraceEnd();

	FHitResult HitResult = GetFirstObjectPhysicsInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("RELEASE"));

	// release the physics handle
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstObjectPhysicsInReach() const
{
	FVector LineTraceEnd = GetLineTraceEnd();
	
	// get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	if (DEBUG)
	{
		// Ray cast
		DrawDebugLine(
			GetWorld(),
			PlayerViewPointLocation,
			LineTraceEnd,
			FColor(0, 255, 100),
			false,
			0.f,
			0,
			5.f
		);
	}
	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),  // tag of the tracing
		false,  // whether to trace through complex actors (see through doors)
		GetOwner()  // actor to be ignored (we need to ignore the DefaultPawn, aka the owner since it's the grabber)
	);
	GetWorld()->LineTraceSingleByObjectType(
		Hit,  // First blocking hit found
		PlayerViewPointLocation,  // Start location of the ray
		LineTraceEnd,  // End location of the ray
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // List of object types it's looking for
		TraceParams // other parameters in which we specify to ignore the DefaultPawn to avoid grabbing itself
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)  // saves from crashes
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}

FVector UGrabber::GetLineTraceEnd() const
{
	// get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	if (DEBUG)
	{
		UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
			*PlayerViewPointLocation.ToString(),
			*PlayerViewPointRotation.ToString()
		);
	}

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector LineTraceEnd = GetLineTraceEnd();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}
