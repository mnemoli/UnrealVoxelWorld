#pragma once
#include "PolyVox/MaterialDensityPair.h"
#include "PolyVox/PagedVolume.h"

typedef PolyVox::MaterialDensityPair44 PolyVoxelType;
typedef PolyVox::PagedVolume<PolyVoxelType> PolyVoxVolumeType;
typedef PolyVoxVolumeType::Chunk PolyVoxChunk;
typedef PolyVox::Region PolyVoxRegion;
typedef uint8_t VoxelDataType;
typedef uint8 BPVoxelDataType;