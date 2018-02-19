// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankPlayerController.h"
#include "BattleTank.h"

void  ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Controlling PlayerController Tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possesing: %s"), *(ControlledTank->GetName()))
	}
}
// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}
ATank * ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }
	FVector HitLocation; //out parameter
	if (GetSightRayHitLocation(HitLocation))//raytrace
	{
		GetControlledTank()->AimAt(HitLocation);
		//TODO tell controlled tank to aim at this point
	}
	//get world location if linetrace through crosshair
		//tell controlled tank to aim at this point
		
}
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find Crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
		
	//de-project the screen position of the crosshair to a world direction
	
	FVector LookDirection;
	if(GetLookDirection(ScreenLocation, LookDirection))
	{
		//line trace along that lookdirection and see what we hit
		GetLookVectorLocation(LookDirection, HitLocation);


		//UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
	}
	
	return true;
}
bool ATankPlayerController::GetLookVectorLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		 ECollisionChannel::ECC_Visibility))
		{
		HitLocation = HitResult.Location;
			return true;
		}
	HitLocation = FVector(0);
	return false;
}



bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	//de-project the screen position of the crosshair to a world direction
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(	
									ScreenLocation.X, 
									ScreenLocation.Y, 
									CameraWorldLocation,
									LookDirection);
	
	
}



