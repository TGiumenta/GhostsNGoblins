/* ------------------------------------------------------------------------------------------------
Title			: Xtreme Ghosts n Goblins
Written by		: Tony Giumenta
Date			: 12 / 08 / 20
Description		: A series of functions that assists with all things mathematics. Contains a simpler 
                  and more universal clamping method, a calculation for the number of digits, and both 
                  random methods given by Dr. Tom Carbone.
--------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include "baseTypes.h"
#include "MathHelper.h"

// Clamps value by min/max
double Clamp(const double value, const double upper, const double lower) noexcept
{
    return min(upper, max(value, lower));
}

// Calculates the number of digits of the parameter
int32_t NumberDigits(const int value) noexcept
{
    int digits = 0;
    int tempValue = value;

    if (value < 0) { return 0; }

    do
    {
        tempValue /= 10;
        ++digits;
    } while (tempValue != 0);
    
    return digits;
}

// Gets a floating point number within the min (inclusive) and max (exclusive)
float_t GetRangedRandom(const float_t min, const float_t max) noexcept
{
    const int32_t r = rand();
    float_t r1 = static_cast<float_t>(r) / static_cast<float_t>(RAND_MAX);

    r1 *= (max - min);
    r1 += min;

    return r1;
}

// Gets an integer within the min (inclusive) and max (exclusive)
int32_t GetRangedRandom(const int32_t min, const int32_t max) noexcept
{
    const int32_t r = rand();

    float_t r2 = static_cast<float_t>(r) / static_cast<float_t>(RAND_MAX);

    r2 *= (static_cast<float_t>(max) - static_cast<float_t>(min));
    int32_t r1 = static_cast<int32_t>(r2);

    r1 += min;

    return r1;
}

// Returns whether or not value is within the range min (inclusive) max (inclusive)
bool8_t IsInRange(const float_t value, const int32_t min, const int32_t max) noexcept
{
    return ((value >= min) && (value <= max));
}