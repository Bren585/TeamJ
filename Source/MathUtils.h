#pragma once
#include <stdlib.h>

class MathUtils {
public:
	inline static float RandomRange(float min, float max) { return min + (min - max) * (static_cast<float>(rand()) / RAND_MAX); }
};