// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AABB.generated.h"

USTRUCT()
struct ZWENTENDORF_API FBoundaries
{
	GENERATED_USTRUCT_BODY()

	float Right;
	float Left;
	float Top;
	float Bottom;

	FBoundaries(float right, float left, float top, float bottom)
	{
		Right = right;
		Left = left;
		Top = top;
		Bottom = bottom;
	};

	FBoundaries()
	{
		Right = 0;
		Left = 0;
		Top = 0;
		Bottom = 0;
	};
};

USTRUCT()
struct ZWENTENDORF_API FCorners
{
	GENERATED_USTRUCT_BODY()

	FVector TopLeft;
	FVector TopRight;
	FVector BottomLeft;
	FVector BottomRight;

	FCorners(FVector topLeft, FVector topRight,  FVector bottomLeft, FVector bottomRight)
	{
		TopLeft = topLeft;
		TopRight = topRight;
		BottomLeft = bottomLeft;
		BottomRight = bottomRight;
	};

	FCorners()
	{
		TopLeft = FVector::ZeroVector;
		TopRight = FVector::ZeroVector;
		BottomLeft = FVector::ZeroVector;
		BottomRight = FVector::ZeroVector;
	};
};
/*
 *
 */
UCLASS()
class ZWENTENDORF_API UAABB : public UObject
{
	GENERATED_BODY()

public:
	bool ContainsPoint(float x, float y);
	FBoundaries GetBoundaries();
	FCorners GetCorners();
	TArray<UAABB*> ChildAABBs;
	bool HasChildren;

	UAABB();
	UAABB(FVector center, FVector dimensions);

private:
	FVector Center;
	FVector Dimensions;
};

