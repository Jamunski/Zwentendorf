// Fill out your copyright notice in the Description page of Project Settings.

#include "AABB.h"

bool UAABB::ContainsPoint(float x, float y)
{
	FBoundaries boundaries = GetBoundaries();

	return boundaries.Left <= x &&
		boundaries.Right >= x &&
		boundaries.Top >= y &&
		boundaries.Bottom <= y;
}

FBoundaries UAABB::GetBoundaries()
{
	return FBoundaries(
		Center.X + Dimensions.X, //left
		Center.X - Dimensions.X, //right
		Center.Y + Dimensions.Y, //top
		Center.Y - Dimensions.Y); //bottom
}

FCorners UAABB::GetCorners()
{
	return FCorners(
		FVector(Center.X - Dimensions.X, Center.Y + Dimensions.Y, 0.0f), //top left
		FVector(Center.X + Dimensions.X, Center.Y + Dimensions.Y, 0.0f), //top right
		FVector(Center.X - Dimensions.X, Center.Y - Dimensions.Y, 0.0f), //bottom left
		FVector(Center.X + Dimensions.X, Center.Y - Dimensions.Y, 0.0f) //bottom right
	);															
}

UAABB::UAABB(FVector center, FVector dimensions)
{
	Center = center;
	Dimensions = dimensions;
}

UAABB::UAABB()
{
	Center = FVector::ZeroVector;
	Dimensions = FVector::ZeroVector;
}