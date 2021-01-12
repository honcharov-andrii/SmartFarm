#pragma once

#include "Interfaces/IDevice.hpp"

struct SDiscreteDeviceParams : public IDeviceParams
{
  SDiscreteDeviceParams() : IDeviceParams()
  , mDiscreteDevicePin()
  {}

  ~SDiscreteDeviceParams()
  {}

  SDiscreteDeviceParams(const SDiscreteDeviceParams& other) : IDeviceParams()
  , mDiscreteDevicePin(other.mDiscreteDevicePin)
  {}

  SDiscreteDeviceParams& operator=(const SDiscreteDeviceParams& other)
  {
    if (&other != this) 
    {
      mDiscreteDevicePin = other.mDiscreteDevicePin;
    }

    return *this;
  }

  virtual String getType() const
  {
    return "SDiscreteDeviceParams";
  }
  
  uint8_t mDiscreteDevicePin;
};


class CDiscreteDevice : public IDevice
{
public:

CDiscreteDevice() : IDevice()
, mParams()
, mIsDeviceStarting(false)
{
}

~CDiscreteDevice()
{
  stopDevice();
}

virtual void startDevice(const IDeviceParams & params)
{
  if(true == params.getType().equals("SDiscreteDeviceParams"))
  {
    mParams = (const SDiscreteDeviceParams&)(params);
    
    pinMode(mParams.mDiscreteDevicePin, OUTPUT);
    digitalWrite(mParams.mDiscreteDevicePin, HIGH);
    mIsDeviceStarting = true;
  }
}

virtual void stopDevice()
{
  if(true == mIsDeviceStarting)
  {
    digitalWrite(mParams.mDiscreteDevicePin, LOW);
    mIsDeviceStarting = false;
  }
}

virtual bool isDeviceStarting()
{
  return mIsDeviceStarting;
}

private:

SDiscreteDeviceParams mParams;
bool mIsDeviceStarting;
};
