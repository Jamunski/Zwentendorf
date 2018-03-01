// Fill out your copyright notice in the Description page of Project Settings.

#include "QuadTreeSetup.h"
#include "AABB.h"

UAABB* UQuadTreeSetup::NewQuadCell(FVector center, FVector half)
{
	UAABB* createdAABB = new UAABB(center, half);

	AABBs.Add(createdAABB);

	return createdAABB;
}

void UQuadTreeSetup::Setup(FRandomStream randomStream, FVector position)
{
	Settings.RandomStream = randomStream;

	MapDimensions *= FVector(100.0f, 100.0f, 1.0f);
	WorldPosition = position;

	NewQuadCell(WorldPosition + MapDimensions / 2, MapDimensions / 2);

	int sectorBoundaries = Settings.RoomWallBorder * 2 + Settings.SliceRoomMinSize;
	FVector dimensionsOver100 = Settings.Dimensions / 100.0f;
	int roomMaxAmount = FMath::Ceil( (dimensionsOver100.X / sectorBoundaries) * (dimensionsOver100.Y / sectorBoundaries) * 0.75f);
	
	int sliceTries = 0;
	UAABB* currentAABB;

	for (int i = 0; i < roomMaxAmount; i++)
	{
		SliceBreak = true;
		
		if (AABBs.Num() < roomMaxAmount)
			break;
		
		currentAABB = AABBs[i];
		sliceTries = 0;
		while(sliceTries < SliceMaxTries)
		{
			for (int j = 0; j < SliceMaxTries; j++)
			{
				sliceTries = j;
				bool canDropQuad = randomStream.RandRange(1, 100) <= Settings.ChanceToDropQuads;

				if (i >= 4 && !canDropQuad)
					break;

				GenerateQuads(currentAABB);
			}

			SliceBreak = false;
		}
	}

	for (int i = 0; i < AABBs.Num(); i++)
	{
		currentAABB = AABBs[i];
		if (currentAABB->HasChildren)
		{
			TrunkNodes.Add(currentAABB);
		}
		else
		{
			LeafNodes.Add(currentAABB);
		}
	}
}

void UQuadTreeSetup::GenerateQuads(UAABB* aabb)
{
	FVector sliceResult = RandomSlice(aabb);
	FBoundaries boundaries = aabb->GetBoundaries();

	float roomSize = (Settings.SliceRoomMinSize + Settings.RoomWallBorder) * Settings.TileSize;

	if (FMath::Abs(boundaries.Left   - sliceResult.X) >= roomSize &&
		FMath::Abs(boundaries.Right  - sliceResult.X) >= roomSize &&
		FMath::Abs(boundaries.Top    - sliceResult.Y) >= roomSize &&
		FMath::Abs(boundaries.Bottom - sliceResult.Y) >= roomSize)
	{
		SliceBreak = true;
		Slices = true;

		auto cellDimensions = CellSize(sliceResult.X, sliceResult.Y, aabb);
		auto cellCenter = CellCenter(sliceResult.X, sliceResult.Y, cellDimensions);

		for (int i = 0; i < 4; i++)
		{
			auto newQuad = NewQuadCell(cellCenter[i], cellDimensions[i]);
			aabb->ChildAABBs.Add(newQuad);
		}

		aabb->HasChildren = true;
	}
	else
	{
		//VV-TODO: handle the case when a room cannot be inserted somehow?
	}
}

TArray<FVector> UQuadTreeSetup::CellSize(float horizonalSlice, float verticalSlice, UAABB* aabb)
{
	FBoundaries boundaries = aabb->GetBoundaries();
	
	float distanceLeft = horizonalSlice - boundaries.Left;
	float distanceRight = boundaries.Right - horizonalSlice;
	float distanceTop = boundaries.Top - horizonalSlice;
	float distanceBottom = horizonalSlice - boundaries.Bottom;
	
	TArray<FVector> distanceVectors;
	distanceVectors.Add(FVector(distanceLeft, distanceTop, 1));
	distanceVectors.Add(FVector(distanceRight, distanceTop, 1));
	distanceVectors.Add(FVector(distanceLeft, distanceBottom, 1));
	distanceVectors.Add(FVector(distanceRight, distanceBottom, 1));

	return distanceVectors;
}

TArray<FVector> UQuadTreeSetup::CellCenter(float horizonalSlice, float verticalSlice, TArray<FVector> dimensions)
{
	//The division by 1 was in the original blueprint, I am very confused as to why, but I left it here in case it actually does something

	TArray<FVector> centerVectors;
	centerVectors.Add(FVector(horizonalSlice - dimensions[0].X, verticalSlice + dimensions[0].Y, 0));
	centerVectors.Add(FVector(horizonalSlice + dimensions[1].X, verticalSlice + dimensions[1].Y, 0));
	centerVectors.Add(FVector(horizonalSlice - dimensions[2].X, verticalSlice - dimensions[2].Y, 0));
	centerVectors.Add(FVector(horizonalSlice - dimensions[3].X, verticalSlice - dimensions[3].Y, 0));

	return centerVectors;
}

FVector UQuadTreeSetup::RandomSlice(UAABB* aabb)
{
	FBoundaries boundaries = aabb->GetBoundaries();

	int horizontalSlice = Settings.RandomStream.RandRange(FMath::Floor(boundaries.Left)/Settings.TileSize,
		FMath::Floor(boundaries.Right) / Settings.TileSize);
	int verticalSlice = Settings.RandomStream.RandRange(FMath::Floor(boundaries.Left) / Settings.TileSize,
		FMath::Floor(boundaries.Right) / Settings.TileSize);

	return FVector(horizontalSlice * Settings.TileSize, verticalSlice * Settings.TileSize,0.0f);
}


UQuadTreeSetup::UQuadTreeSetup(FQuadTreeInfo info)
{
	Settings = info;
}

UQuadTreeSetup::UQuadTreeSetup()
{
	Settings = FQuadTreeInfo();
}


