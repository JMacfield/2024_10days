#pragma once
#include <random>

class Random
{
public:
	static void Initialize();

	static int GetRandomInt(int min, int max);

	static float GetRandomFloat(float min, float max);

private:
	static std::mt19937 randomEngine_;
};

