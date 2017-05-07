#pragma once
#include "NoiseGenerator.h"
#include "FastNoiseGenerator.generated.h"

class UUFNNoiseGenerator;

UCLASS()
class VOXELWORLD_API UFastNoiseGenerator : public UNoiseGenerator
{
	GENERATED_BODY()
	UFastNoiseGenerator();
	virtual float GetNoise(float x, float y, float z); 
	virtual float GetNoise(float x, float y);
	virtual void Setup(float Seed, float Octaves, float Frequency, float Scale);

private:
	UPROPERTY() UUFNNoiseGenerator *Noise;
	UPROPERTY() float Scale;
};
