/*
 * Texel.cpp
 *
 *  Created on: Jun 9, 2019
 *      Author: Antero Nousiainen
 */

#include "Texel.hpp"
#include <tuple>

using namespace image;

bool Texel::operator==(const Texel & other) const
{
    return std::tie(referenceColors, lookupTable) == std::tie(other.referenceColors, other.lookupTable);
}
