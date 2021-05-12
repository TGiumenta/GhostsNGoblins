#pragma once
#include "baseTypes.h"

double Clamp(const double value, const double upper, const double lower) noexcept;
int32_t NumberDigits(const int value) noexcept;
float_t GetRangedRandom(const float_t min, float_t max) noexcept;
int32_t GetRangedRandom(const int32_t min, int32_t max) noexcept;
bool8_t IsInRange(const float_t value, const int32_t min, const int32_t max) noexcept;