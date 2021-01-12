#pragma once

#include "Interfaces/ISensor.hpp"

struct STemperatureSensorData : public ISensorData
{
  STemperatureSensorData() : ISensorData()
  , mTemperature()
  {}

  ~STemperatureSensorData()
  {}

  STemperatureSensorData(const STemperatureSensorData& other) : ISensorData()
  , mTemperature(other.mTemperature)
  {}

  STemperatureSensorData& operator=(const STemperatureSensorData& other)
  {
    if (&other != this) 
    {
        mTemperature = other.mTemperature;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "STemperatureSensorData";
  }

  uint8_t mTemperature;
};