// Copyright (C) 2015 Rémi Bèges
// This file is part of ZNoise - a C++ noise library
// For conditions of distribution and use, see LICENSE file

#include "InvertedWorley.hpp"

InvertedWorley::InvertedWorley(unsigned int seed) : Worley(seed)
{
}

float InvertedWorley::Get(std::initializer_list<float> coordinates, float scale) const
{
    return -Worley::Get(coordinates, scale);
}
