// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuadTreeSetup.h"

#include "LevelGrid.generated.h"

class UInstancedStaticMeshComponent;
class UAABB;

UCLASS(Blueprintable)
class ZWENTENDORF_API ALevelGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGrid();

	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* GroundFillerFull;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* CapWall;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* CornerFiller;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* CapFiller;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* PillarWall;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* Floor;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* Wall;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* OutsideCorner;
	UPROPERTY(EditAnywhere)
		UInstancedStaticMeshComponent* InsideCorner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DigRoom(UAABB* aabb);
	void GenerateRoomTiles(UAABB* roomAABB, FRandomStream randomStream);
	int GetMapIndexForXY(float x, float y);
	void EdgeClearing();
	void GenerateWalls();

//	UPROPERTY();
	FQuadTreeInfo LevelInfo;
//	UPROPERTY();
	UAABB* currentAABB;
//	UPROPERTY();
	TArray<UAABB*> rooms;
	//	UPROPERTY();
	TArray<bool> Tiles;
	TArray<int> TileTypes; //VV-TODO: convert to an array of enums?
	TArray<int> CornerRotations;
	FVector GetXYBasedOnIndex(int index, int maxX, int maxY);
	TArray<int> TestNSEW(int x, int y, TArray<int> TileTypes);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(int seed, FQuadTreeInfo levelInfo); //VV-TODO: refactor this so that the params are all in one struct
	void Create(UQuadTreeSetup* UQuadTreeSetup, FRandomStream randomStream);
	
};
