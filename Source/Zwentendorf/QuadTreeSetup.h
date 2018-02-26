// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuadTreeSetup.generated.h"

/**
 * 
 */
class UAABB;

USTRUCT()
struct ZWENTENDORF_API FQuadTreeInfo
{
	GENERATED_USTRUCT_BODY()

	FVector Dimensions;
	int SliceRoomMinSize;
	int TileSize;
	float ChanceToDropQuads;
	int RoomWallBorder;
	FRandomStream RandomStream;

	FQuadTreeInfo(FVector dimensions, int sliceRoomMinSize, int tileSize, float chanceToDropQuad, int roomWallBorder)
	{
		Dimensions = dimensions;
		SliceRoomMinSize = sliceRoomMinSize;
		TileSize = tileSize;
		ChanceToDropQuads = chanceToDropQuad;
		RoomWallBorder = roomWallBorder;
	};

	FQuadTreeInfo()
	{
		Dimensions = FVector::ZeroVector;
		SliceRoomMinSize = 0;
		TileSize = 0;
		ChanceToDropQuads = 0.0f;
		RoomWallBorder = 0;
	};
};

UCLASS()
class ZWENTENDORF_API UQuadTreeSetup : public UObject
{
	GENERATED_BODY()
	
public:
	UQuadTreeSetup(FQuadTreeInfo info);
	UQuadTreeSetup();
	void Setup(FRandomStream randomStream, FVector position);

private:
	FVector MapDimensions;
	FVector WorldPosition;
	UAABB* NewQuadCell(FVector center, FVector half);
	TArray<UAABB*> AABBs;
	FQuadTreeInfo Settings;

	int SliceMaxTries = 25;

	bool Slices;

	void GenerateQuads(UAABB* aabb);
	
	FVector RandomSlice(UAABB* aabb);

};
