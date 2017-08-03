#pragma once

#include "Worley.hpp"
#include "Enums.hpp"
#include <initializer_list>
#include <map>

class InvertedWorley : public Worley
{
    public:
      InvertedWorley() = default;
      InvertedWorley(unsigned int seed);
      ~InvertedWorley() = default;

      float Get(std::initializer_list<float> coordinates, float scale) const override;
};
