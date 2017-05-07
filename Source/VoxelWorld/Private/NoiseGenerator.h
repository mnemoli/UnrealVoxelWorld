#pragma once

#include "UObject/NoExportTypes.h"
#include "NoiseGenerator.generated.h"

/**
 * 
 */
UCLASS(abstract)
class VOXELWORLD_API UNoiseGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual float GetNoise(float x, float y, float z) PURE_VIRTUAL(UNoiseGenerator::GetNoise, return 0;)
	virtual float GetNoise(float x, float y) PURE_VIRTUAL(UNoiseGenerator::GetNoise, return 0;)
	
	UFUNCTION()
	virtual void Setup(float Seed, float Octaves, float Frequency, float Scale) PURE_VIRTUAL(UNoiseGenerator::Setup, );
};
