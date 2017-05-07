#include "VoxelWorld.h"
#include "VoxelTerrainPager.h"
#include "FastNoiseGenerator.h"

using namespace PolyVox;

VoxelTerrainPager::VoxelTerrainPager(UNoiseGenerator* NoiseGenerator)
	: PolyVoxVolumeType::Pager(), NoiseGenerator{ NoiseGenerator }
{
}

VoxelTerrainPager::~VoxelTerrainPager()
{
}

// Called when a new chunk is paged in
// This function will automatically generate our voxel-based terrain from simplex noise
void VoxelTerrainPager::pageIn(const PolyVoxRegion& region, PolyVoxChunk* Chunk) 
{
	generateFromNoise(region, Chunk);
}

// Called when a chunk is paged out
void VoxelTerrainPager::pageOut(const PolyVoxRegion& region, PolyVoxChunk* Chunk) 
{

}

void VoxelTerrainPager::generateFromNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk)
{
	generateFrom2DNoise(region, Chunk);
}

void VoxelTerrainPager::generateFrom2DNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk) 
{
	check(NoiseGenerator != nullptr);

	const float regionZHeight = region.getUpperZ() - region.getLowerZ();

	PolyVoxelType Voxel;
	Voxel.setDensity(255);
	Voxel.setMaterial(1);

	//Apply noise to each voxel in the chunk
	for (int x = region.getLowerX(); x <= region.getUpperX(); x++)
	{
		for (int y = region.getLowerY(); y <= region.getUpperY(); y++)
		{
			auto NoiseValue = NoiseGenerator->GetNoise(x, y) + (region.getLowerZ() + (regionZHeight/2) );

			check(NoiseValue > region.getLowerZ() && NoiseValue < region.getUpperZ());

			for (int z = region.getLowerZ(); z <= NoiseValue; z++)
			{
				Chunk->setVoxel(x - region.getLowerX(), y - region.getLowerY(), z - region.getLowerZ(), Voxel);
			}
		}
	}
}

void VoxelTerrainPager::generateFrom3DNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk)
{
	check(NoiseGenerator != nullptr);

	PolyVoxelType Voxel;

	for (int x = region.getLowerX(); x <= region.getUpperX(); x++)
	{
		for (int y = region.getLowerY(); y <= region.getUpperY(); y++)
		{
			for (int z = region.getLowerZ(); z <= region.getUpperZ(); z++)
			{
				auto NoiseValue = NoiseGenerator->GetNoise(x, y, z);
				if (NoiseValue > 0.5)
				{
					Voxel.setDensity(255);
					Voxel.setMaterial(1);
				}
				else
				{
					Voxel.setDensity(0);
					Voxel.setMaterial(0);
				}
				Chunk->setVoxel(x - region.getLowerX(), y - region.getLowerY(), z - region.getLowerZ(), Voxel);
			}
		}
	}
}