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
	bool sliceBreak;
	for (int i = 0; i < roomMaxAmount; i++)
	{
		sliceBreak = true;
		
		if (AABBs.Num() < roomMaxAmount)
			break;
		
		auto currentAABB = AABBs[i];
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

			sliceBreak = false;
		}
	}
}

void UQuadTreeSetup::GenerateQuads(UAABB* aabb)
{
	FVector sliceResult = RandomSlice(aabb);

	//VV-TODO: finish this function

}

FVector UQuadTreeSetup::RandomSlice(UAABB* aabb)
{
	FBoundaries boundaries = aabb->GetBoundaries();

	int horizontalSlice = Settings.RandomStream.RandRange(FMath::Floor(boundaries.Left)/Settings.TileSize,
		FMath::Floor(boundaries.Right) / Settings.TileSize);
	int verticalSlice = Settings.RandomStream.RandRange(FMath::Floor(boundaries.Left) / Settings.TileSize,
		FMath::Floor(boundaries.Right) / Settings.TileSize);

	return FVector(horizontalSlice * Settings.TileSize, verticalSlice * Settings.TileSize);
}


UQuadTreeSetup::UQuadTreeSetup(FQuadTreeInfo info)
{
	Settings = info;
}

UQuadTreeSetup::UQuadTreeSetup()
{
	Settings = FQuadTreeInfo();
}


