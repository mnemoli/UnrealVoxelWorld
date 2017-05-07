#include "VoxelWorld.h"
#include "PolyvoxTerrain.h"
#include "PolyVox/PagedVolume.h"
#include "VoxelTerrainPager.h"
#include "PolyVox/CubicSurfaceExtractor.h"
#include "PolyVox/Mesh.h"
#include "FastNoiseGenerator.h"

APolyvoxTerrain::APolyvoxTerrain()
{
	// Default values for our noise control variables.
	Seed = 123;
	NoiseOctaves = 3;
	NoiseFrequency = 0.01f;
	NoiseScale = 32.f;
	BlockSize = 10;
	ChunkSize = 16;

	NoiseGenerator = CreateDefaultSubobject<UFastNoiseGenerator>(TEXT("Noise Generator"));
}

void APolyvoxTerrain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	NoiseGenerator->Setup(Seed, NoiseOctaves, NoiseFrequency, NoiseScale);
	VoxelVolume = MakeUnique<PolyVoxVolumeType>(new VoxelTerrainPager(NoiseGenerator));
}

void APolyvoxTerrain::GenerateSampleTerrain()
{
	RenderSampleTerrain();
}

void APolyvoxTerrain::RenderSampleTerrain()
{
	int32 MeshSection = 0;
	for (int x = 0; x <= 3; x++)
	{
		for (int y = 0; y <= 3; y++)
		{
			RenderMesh(ExtractMesh(FVector(x * BlockSize * ChunkSize, y * BlockSize * ChunkSize, 0)), MeshSection);
			MeshSection++;
		}
	}
}

void APolyvoxTerrain::GetVoxel(const FVector& WorldVector, uint8& Material, uint8& Density)
{
	auto Voxel = GetVoxelRaw(WorldVector);
	Material = Voxel.getMaterial();
	Density = Voxel.getDensity();
}

bool APolyvoxTerrain::SetVoxel(const FVector& WorldVector, uint8 Material, uint8 Density)
{
	PolyVoxelType Voxel;
	Voxel.setMaterial(Material);
	Voxel.setDensity(Density);
	try
	{
		VoxelVolume->setVoxel(ToPolyVector(WorldVector), Voxel);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

PolyVoxelType APolyvoxTerrain::GetVoxelRaw(const FVector& WorldVector)
{
	return VoxelVolume->getVoxel(ToPolyVector(WorldVector));
}

FDecodedMesh* APolyvoxTerrain::ExtractMesh(const FVector& WorldOrigin)
{
	FDecodedMesh* Mesh = new FDecodedMesh();

	auto Origin = ToPolyVector(WorldOrigin);

	PolyVox::Region ToExtract = PolyVox::Region(Origin, Origin + PolyVox::Vector3DInt32(ChunkSize));

	CriticalSection.Lock();
	auto ExtractedMesh = extractCubicMesh(VoxelVolume.Get(), ToExtract);
	CriticalSection.Unlock();

	auto DecodedMesh = decodeMesh(ExtractedMesh);

	if (DecodedMesh.getNoOfIndices() == 0) {
		return Mesh;
	}

	auto AddToMeshData = [DecodedMesh, Mesh, WorldOrigin, this](uint32 i) {
		auto Index = DecodedMesh.getIndex(i);
		auto Vertex = DecodedMesh.getVertex(Index);
		auto TriangleMaterial = Vertex.data.getMaterial();

		FVector VertexVector = ToFVector(Vertex.position);
		Mesh->Indices.Add(Mesh->Vertices.Add(VertexVector + WorldOrigin));

		return Vertex;
	};

	auto CalculateEdge = [this](auto Vertex1, auto Vertex2) {
		return ToFVector(Vertex1.position - Vertex2.position);
	};

	// Loop over all of the triangle vertex indices
	for (uint32 i = 0; i < DecodedMesh.getNoOfIndices() - 2; i += 3)
	{
		// We need to add the vertices of each triangle in reverse or the mesh will be upside down
		const auto Vertex2 = AddToMeshData(i + 2);
		const auto Vertex1 = AddToMeshData(i + 1);
		const auto Vertex0 = AddToMeshData(i);

		// Calculate the tangents of our triangle
		const FVector Edge01 = CalculateEdge(Vertex1, Vertex0);
		const FVector Edge02 = CalculateEdge(Vertex2, Vertex0);

		const FVector TangentX = Edge01.GetSafeNormal();
		const FVector TangentZ = (Edge01 ^ Edge02).GetSafeNormal();

		for (int32 x = 0; x < 3; x++)
		{
			Mesh->Tangents.Add(FRuntimeMeshTangent(TangentX, false));
			Mesh->Normals.Add(TangentZ);
		}
	}

	return Mesh;
}

void APolyvoxTerrain::RenderMesh(FDecodedMesh* Mesh, int32 MeshSection)
{
	if (Mesh == nullptr || Mesh->Vertices.Num() == 0)
	{
		return;
	}
	TerrainMesh->CreateMeshSection(MeshSection, Mesh->Vertices, Mesh->Indices, Mesh->Normals, Mesh->UV0, Mesh->Colors, Mesh->Tangents, true, EUpdateFrequency::Frequent);
}

PolyVox::Vector3DInt32 APolyvoxTerrain::ToPolyVector(const FVector& WorldVector)
{
	if (BlockSize == 0)
	{
		return PolyVox::Vector3DInt32(0, 0, 0);
	}
	auto PolyVector = PolyVox::Vector3DInt32(WorldVector.X, WorldVector.Y, WorldVector.Z);
	return PolyVector / BlockSize;
}

FVector APolyvoxTerrain::ToFVector(const PolyVox::Vector3DInt32& PolyvoxVector)
{
	FVector Vec = FVector(PolyvoxVector.getX(), PolyvoxVector.getY(), PolyvoxVector.getZ());
	return Vec * BlockSize;
}

FVector APolyvoxTerrain::ToFVector(const PolyVox::Vector3DFloat& PolyvoxVector)
{
	FVector Vec = FVector(PolyvoxVector.getX(), PolyvoxVector.getY(), PolyvoxVector.getZ());
	return Vec * BlockSize;
}