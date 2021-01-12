#pragma once

#include "Interfaces/ISensor.hpp"
#include "STemperatureSensorData.hpp"

#include <OneWire.h>
#include <DallasTemperature.h>

struct SOneWireTemperatureSensorParams : public ISensorParams
{
  SOneWireTemperatureSensorParams() : ISensorParams()
  , mDeviceAddress()
  , mSensors()
  {}

  ~SOneWireTemperatureSensorParams()
  {}

  SOneWireTemperatureSensorParams(const SOneWireTemperatureSensorParams& other) : ISensorParams()
  , mDeviceAddress()
  , mSensors(other.mSensors)
  {
    for(uint8_t i = 0; i < 8; ++i)
    {
        mDeviceAddress[i] = other.mDeviceAddress[i];
    }
  }

  SOneWireTemperatureSensorParams& operator=(const SOneWireTemperatureSensorParams& other)
  {
    if (&other != this) 
    {
      for(uint8_t i = 0; i < 8; ++i)
      {
        mDeviceAddress[i] = other.mDeviceAddress[i];
      }
      
  		mSensors = other.mSensors;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "SOneWireTemperatureSensorParams";
  }

  DeviceAddress mDeviceAddress;
  DallasTemperature mSensors;
};

class COneWireTemperatureSensor : public ISensor
{
public:
	COneWireTemperatureSensor(ISensorParams & sensorParams) : ISensor()
	, mParams()
  , mLastTempSensorData()
	, mIsInit(false)
	{
	  if(true == sensorParams.getType().equals("SOneWireTemperatureSensorParams"))
	  {
	    mParams = (const SOneWireTemperatureSensorParams&)(sensorParams);
	    mIsInit = true;
	  }
	}

	~COneWireTemperatureSensor()
	{}

	virtual bool isInit()
	{
		return mIsInit;
	}
	
	virtual const ISensorData & getData()
	{  
		if(true == mIsInit)
		{
			mParams.mSensors.requestTemperatures();

			mLastTempSensorData.mTemperature = mParams.mSensors.getTempC(mParams.mDeviceAddress);
		}

		return (const ISensorData &)(mLastTempSensorData);
	}

private:

SOneWireTemperatureSensorParams mParams;
STemperatureSensorData mLastTempSensorData;
bool mIsInit;
};
