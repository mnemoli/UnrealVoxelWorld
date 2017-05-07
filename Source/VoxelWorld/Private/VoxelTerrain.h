#pragma once

#include "GameFramework/Actor.h"
#include "GlobalDefs.h"
#include "VoxelTerrain.generated.h"

class URuntimeMeshComponent;

UCLASS(abstract)
class VOXELWORLD_API AVoxelTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelTerrain();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/**
	Test the generation of a sample terrain
	Using whatever method the subclass defines
	**/
	UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
	virtual void GenerateSampleTerrain() PURE_VIRTUAL(AVoxelTerrain::GenerateSampleTerrain, );

	// Retrieve values for a singel voxel
	UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
	virtual void GetVoxel(const FVector& WorldVector, uint8& Material, uint8& Density) PURE_VIRTUAL(AVoxelTerrain::GetVoxel, );

	// Set a single voxel
	UFUNCTION(Category = "Voxel Terrain", BlueprintCallable)
	virtual bool SetVoxel(const FVector& VoxelLocation, uint8 Material, uint8 Density) PURE_VIRTUAL(AVoxelTerrain::SetVoxel, return false;);

protected:
	UPROPERTY() class URuntimeMeshComponent* TerrainMesh;
	
};
