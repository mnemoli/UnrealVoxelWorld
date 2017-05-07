#pragma once
#include "GlobalDefs.h"
#include "PolyVox/PagedVolume.h"

class UNoiseGenerator;

class VOXELWORLD_API VoxelTerrainPager : public PolyVoxVolumeType::Pager
{
public:
	VoxelTerrainPager(UNoiseGenerator* NoiseGenerator);
	~VoxelTerrainPager();

	// PagedVolume::Pager functions
	virtual void pageIn(const PolyVoxRegion& region, PolyVoxChunk* pChunk);
	virtual void pageOut(const PolyVoxRegion& region, PolyVoxChunk* pChunk);

private:
	void generateFromNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk);
	void generateFrom2DNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk);
	void generateFrom3DNoise(const PolyVoxRegion& region, PolyVoxChunk* Chunk);

private:
	UNoiseGenerator* NoiseGenerator;
};
