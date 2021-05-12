#ifndef RANDOM_HEADER	// Header Guard
#define RANDOM_HEADER

#include "baseTypes.h"

float_t		GetRangedRandom(float_t const min, float_t const max) noexcept;
int32_t		GetRangedRandom(int32_t const min, int32_t const max) noexcept;

#endif