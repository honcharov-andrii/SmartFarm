#pragma once

#include "Interfaces/ISensor.hpp"

struct SDistanceSensorData : public ISensorData
{
  SDistanceSensorData() : ISensorData()
  , mDistance()
  {}

  ~SDistanceSensorData()
  {}

  SDistanceSensorData(const SDistanceSensorData& other) : ISensorData()
  , mDistance(other.mDistance)
  {}

  SDistanceSensorData& operator=(const SDistanceSensorData& other)
  {
    if (&other != this) 
    {
        mDistance = other.mDistance;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "SDistanceSensorData";
  }

  uint8_t mDistance;
};