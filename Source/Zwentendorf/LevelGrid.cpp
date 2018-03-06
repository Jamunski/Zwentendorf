// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGrid.h"
#include "AABB.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
ALevelGrid::ALevelGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALevelGrid::Initialize(int seed, FQuadTreeInfo levelInfo)
{
	LevelInfo = levelInfo;

	//VV-TODO: the original blueprint has a setting for turning shadows on/off for level pieces, decide if this is necessary
	TArray<UInstancedStaticMeshComponent*> instancedMeshes;
	this->GetComponents<UInstancedStaticMeshComponent>(instancedMeshes);

	for (int i = 0; i < instancedMeshes.Num(); i++)
	{
		instancedMeshes[i]->SetCullDistances(5000, 6000); //This was hardcoded in the original blueprint
	}

	//VV-TODO: the original blueprint resizes the array variables to match the map dimensions and also initializes every element, not sure if needed here since TArray is a dynamic array?


}

void ALevelGrid::Create(UQuadTreeSetup* quadTreeSetup, FRandomStream randomStream)
{
	for (int i = 0; i < quadTreeSetup->AABBs.Num(); i++)
	{
		//Generate room tiles
		GenerateRoomTiles(quadTreeSetup->AABBs[i], randomStream);
	}

	EdgeClearing();

	//Finish from here
}

void ALevelGrid::GenerateRoomTiles(UAABB* roomAABB, FRandomStream randomStream)
{
	currentAABB = roomAABB;

	if (!roomAABB->HasChildren)
	{
		bool okForTiles = false;

		FBoundaries roomBoundariesInTiles(
			roomAABB->GetCorners().TopLeft.X / LevelInfo.TileSize, //right
			roomAABB->GetCorners().BottomRight.X / LevelInfo.TileSize, //left
			roomAABB->GetCorners().TopLeft.Y / LevelInfo.TileSize, //top
			roomAABB->GetCorners().BottomRight.Y / LevelInfo.TileSize //bottom
		);

		FVector roomCenter;
		FVector roomDimensions;

		for (int i = 0; i < 14; i++)
		{
			FVector minCenter = FVector(
				FMath::Floor( LevelInfo.RoomWallBorder + roomBoundariesInTiles.Left + LevelInfo.SliceRoomMinSize / 2),
				FMath::Floor(LevelInfo.RoomWallBorder + roomBoundariesInTiles.Top + LevelInfo.SliceRoomMinSize / 2),
				1
			);

			FVector maxCenter = FVector(
				FMath::Floor(LevelInfo.RoomWallBorder - roomBoundariesInTiles.Right - LevelInfo.SliceRoomMinSize / 2),
				FMath::Floor(LevelInfo.RoomWallBorder - roomBoundariesInTiles.Bottom - LevelInfo.SliceRoomMinSize / 2),
				1
			);

			roomCenter = FVector(
				randomStream.RandRange(minCenter.X, maxCenter.X),
				randomStream.RandRange(minCenter.Y, maxCenter.Y),
				0
			);

			float halfLeft = roomCenter.X - roomBoundariesInTiles.Left - LevelInfo.RoomWallBorder;
			float halfRight= roomBoundariesInTiles.Right - roomCenter.X - LevelInfo.RoomWallBorder;
			float halfBottom = roomCenter.Y - roomBoundariesInTiles.Bottom - LevelInfo.RoomWallBorder;
			float halfTop = roomBoundariesInTiles.Top - roomCenter.Y - LevelInfo.RoomWallBorder;

			float halfX = halfLeft < halfRight ? halfLeft : halfRight;
			halfX = halfX > LevelInfo.SliceRoomMinSize / 2 ? LevelInfo.SliceRoomMinSize / 2 : halfX;

			float halfY = halfTop < halfBottom ? halfTop : halfBottom;
			halfY = halfY > LevelInfo.SliceRoomMinSize / 2 ? LevelInfo.SliceRoomMinSize / 2 : halfY;

			roomDimensions = FVector(
				FMath::Round( randomStream.RandRange(LevelInfo.SliceRoomMinSize / 2, FMath::Ceil(halfX))),
				FMath::Round(randomStream.RandRange(LevelInfo.SliceRoomMinSize / 2, FMath::Ceil(halfY))),
				0
			);

			roomDimensions *= LevelInfo.TileSize;
			roomCenter *= LevelInfo.TileSize;

			bool atLeast2x2 = roomDimensions.X * 2 >= (LevelInfo.TileSize * 2) && roomDimensions.Y * 2 >= (LevelInfo.TileSize * 2);
			bool fitsInAABB = 
				roomAABB->GetCorners().BottomLeft.X <= (roomCenter.X - roomDimensions.X) &&
				roomAABB->GetCorners().BottomLeft.Y <= (roomCenter.Y - roomDimensions.Y) &&
				roomAABB->GetCorners().TopRight.X >= (roomDimensions.X + roomCenter.X) &&
				roomAABB->GetCorners().TopRight.Y >= (roomDimensions.Y + roomCenter.Y);

			if (atLeast2x2 && fitsInAABB)
				break;
		}

		if (okForTiles)
		{
			UAABB* newAABB = new UAABB();
			DigRoom(newAABB);
			newAABB->Initialize(roomCenter, roomDimensions);
			currentAABB = newAABB;

			rooms.Add(newAABB);
		}
		
	}
}

void ALevelGrid::DigRoom(UAABB* aabb)
{
	//Swapping right and left and top and bottom (from least to greatest)
	FBoundaries boundaries(
		aabb->GetBoundaries().Right < aabb->GetBoundaries().Left ? aabb->GetBoundaries().Left : aabb->GetBoundaries().Right,
		aabb->GetBoundaries().Right < aabb->GetBoundaries().Left ? aabb->GetBoundaries().Right : aabb->GetBoundaries().Left,
		aabb->GetBoundaries().Top < aabb->GetBoundaries().Bottom ? aabb->GetBoundaries().Bottom : aabb->GetBoundaries().Top,
		aabb->GetBoundaries().Top < aabb->GetBoundaries().Bottom ? aabb->GetBoundaries().Top : aabb->GetBoundaries().Bottom
	);

	int firstIndexVertical = FMath::Floor(aabb->GetBoundaries().Bottom / LevelInfo.TileSize);
	int lastIndexVertical = FMath::Floor(aabb->GetBoundaries().Top / LevelInfo.TileSize) - 1;

	int firstIndexHorizontal = FMath::Floor(aabb->GetBoundaries().Left / LevelInfo.TileSize);
	int lastIndexHorizontal = FMath::Floor(aabb->GetBoundaries().Right / LevelInfo.TileSize) - 1;

	for (int i = firstIndexVertical; i < lastIndexVertical; i++)
	{
		for (int j = firstIndexHorizontal; j < lastIndexHorizontal; j++)
		{
			Tiles[GetMapIndexForXY(j, i)] = true;
			TileTypes[GetMapIndexForXY(j, i)] = 1;
		}
	}
}

int ALevelGrid::GetMapIndexForXY(float x, float y)
{
	return LevelInfo.Dimensions.Y * y + x;
}

void ALevelGrid::EdgeClearing()
{
	for (int i = 0; i < LevelInfo.Dimensions.X; i++)
	{
		Tiles[GetMapIndexForXY(0, i)] = false;
		TileTypes[GetMapIndexForXY(0, i)] = 0;
	}

	for (int i = 0; i < LevelInfo.Dimensions.X; i++)
	{
		Tiles[GetMapIndexForXY(LevelInfo.Dimensions.X, i)] = false;
		TileTypes[GetMapIndexForXY(LevelInfo.Dimensions.X, i)] = 0;
	}

	for (int i = 0; i < LevelInfo.Dimensions.Y; i++)
	{
		Tiles[GetMapIndexForXY(i, 0)] = false;
		TileTypes[GetMapIndexForXY(i, 0)] = 0;
	}

	for (int i = 0; i < LevelInfo.Dimensions.Y; i++)
	{
		Tiles[GetMapIndexForXY(i, LevelInfo.Dimensions.Y)] = false;
		TileTypes[GetMapIndexForXY(i, LevelInfo.Dimensions.Y)] = 0;
	}
}

void ALevelGrid::GenerateWalls()
{
	CornerRotations.Empty();

	for (int i = 0; i < TileTypes.Num(); i++)
	{
		int localIndexX = GetXYBasedOnIndex(i,LevelInfo.Dimensions.X, LevelInfo.Dimensions.Y).X;
		int localIndexY = GetXYBasedOnIndex(i, LevelInfo.Dimensions.X, LevelInfo.Dimensions.Y).Y;

		if (TileTypes[GetMapIndexForXY(localIndexX, localIndexY)]==0)
		{
			TArray<int> points = TestNSEW(localIndexX, localIndexY, TileTypes);

			for (int j = 0; j < points.Num(); j++)
			{
				if (points[j] == 0 || points[j] == 1)
				{
					TileTypes[i] = 99;
					break;
				}
			}
		}

	}
}

TArray<int> ALevelGrid::TestNSEW(int x, int y, TArray<int> TileTypes)
{
	TArray<int> newTiles;
	
	TArray<FVector> testPoints;
	testPoints.Add(FVector(-1, 0, 0));
	testPoints.Add(FVector(-1, 1, 0));
	testPoints.Add(FVector(0, 1, 0));
	testPoints.Add(FVector(1, 1, 0));
	testPoints.Add(FVector(1, 0, 0));
	testPoints.Add(FVector(1, -1, 0));
	testPoints.Add(FVector(0, -1, 0));
	testPoints.Add(FVector(-1, -1, 0));

	for (int i = 0; i < testPoints.Num(); i++)
	{
		if (testPoints[i].X + x < LevelInfo.Dimensions.X &&
			testPoints[i].Y + y < LevelInfo.Dimensions.Y &&
			testPoints[i].X + x >= 0 &&
			testPoints[i].Y + y >= 0)
		{
			newTiles.Add(GetMapIndexForXY(testPoints[i].X + x, testPoints[i].Y + y));
		}
		else
		{
			newTiles.Add(666);
		}
	}

	return newTiles;
}

FVector ALevelGrid::GetXYBasedOnIndex(int index, int maxX, int maxY)
{
	return FVector(index % maxY, index / maxY,0);
}

// Called when the game starts or when spawned
void ALevelGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

