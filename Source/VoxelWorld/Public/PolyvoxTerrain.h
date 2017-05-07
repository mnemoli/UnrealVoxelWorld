#pragma once

#include "VoxelTerrain.h"
#include "GlobalDefs.h"
#include "RuntimeMeshComponent.h"
#include "PolyvoxTerrain.generated.h"

/**
 * Specific implementation of a voxel terrain using PolyVox.
 */

USTRUCT()
struct FDecodedMesh
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() TArray<FVector> Vertices;
	UPROPERTY() TArray<int32> Indices;
	UPROPERTY() TArray<FVector> Normals;
	UPROPERTY() TArray<FVector2D> UV0;
	UPROPERTY() TArray<FColor> Colors;
	UPROPERTY() TArray<FRuntimeMeshTangent> Tangents;
};


UCLASS()
class VOXELWORLD_API APolyvoxTerrain : public AVoxelTerrain
{
	GENERATED_BODY()

public:
	APolyvoxTerrain();

	virtual void PostInitializeComponents() override;

	virtual void GetVoxel(UPARAM(ref) const FVector& WorldVector, uint8& Material, uint8& Density);

	virtual bool SetVoxel(const FVector& VoxelLocation, uint8 Material, uint8 Density);

	/* Utility functions */

	//Converts a world location into a PolyVox vector in voxel space (e.g. 11, 12, 13 -> 1,1,1)
	PolyVox::Vector3DInt32 ToPolyVector(const FVector& WorldVector);

	//Converts a PolyVox vector in voxel space to a world location (e.g. 1,1,1 -> 10,10,10)
	FVector ToFVector(const PolyVox::Vector3DInt32& PolyvoxVector);
	FVector ToFVector(const PolyVox::Vector3DFloat& PolyvoxVector);

	/* Test functions */
	virtual void GenerateSampleTerrain();
	
	void RenderSampleTerrain();

	/**
	* Configurable terrain values
	**/
	// The seed of our fractal
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 Seed;

	// The number of octaves that the noise generator will use
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 NoiseOctaves;

	// The frequency of the noise
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float NoiseFrequency;

	// The scale of the noise. The output of the TerrainFractal is multiplied by this.
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) float NoiseScale;

	//The size of each voxel
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 BlockSize;

	//The size of a full chunk
	UPROPERTY(Category = "Voxel Terrain", BlueprintReadWrite, EditAnywhere) int32 ChunkSize;

private:
	TUniquePtr<PolyVoxVolumeType> VoxelVolume;
	UPROPERTY() class UNoiseGenerator* NoiseGenerator;

	PolyVoxelType GetVoxelRaw(const FVector& WorldVector);

	FDecodedMesh* ExtractMesh(const FVector& WorldOrigin);
	void RenderMesh(FDecodedMesh* Mesh, int32 MeshSection);

	FCriticalSection CriticalSection;

};
