#pragma once

#include "Interfaces/ISensor.hpp"
#include "SDistanceSensorData.hpp"
#include "Ultrasonic.h"

struct SHCDistanceSensorParams : public ISensorParams
{
  SHCDistanceSensorParams() : ISensorParams()
  , mEchoPin()
  , mTrigPin()
  {}

  ~SHCDistanceSensorParams()
  {}

  SHCDistanceSensorParams(const SHCDistanceSensorParams& other) : ISensorParams()
  , mEchoPin(other.mEchoPin)
  , mTrigPin(other.mTrigPin)
  {}

  SHCDistanceSensorParams& operator=(const SHCDistanceSensorParams& other)
  {
    if (&other != this) 
    {
        mEchoPin = other.mEchoPin;
        mTrigPin = other.mTrigPin;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "SHCDistanceSensorParams";
  }

  uint8_t mEchoPin;
  uint8_t mTrigPin; 
};

class CHCDistanceSensor : public ISensor
{
public:
	CHCDistanceSensor(ISensorParams & sensorParams) : ISensor()
	, mParams()
    , mSonar(nullptr)
	, mIsInit(false)
	{
	  if(true == sensorParams.getType().equals("SHCDistanceSensorParams"))
	  {
	    mParams = (const SHCDistanceSensorParams&)(sensorParams);
        mSonar = new Ultrasonic(mParams.mTrigPin, mParams.mEchoPin);
	    mIsInit = true;
	  }
	}

	~CHCDistanceSensor()
	{}

	virtual bool isInit()
	{
		return mIsInit;
	}
	
	virtual const ISensorData & getData()
	{  
		if(true == mIsInit)
		{
			mLastDistanceData.mDistance = mSonar->read();
		}

		return (const ISensorData &)(mLastDistanceData);
	}

private:

SHCDistanceSensorParams mParams;
Ultrasonic* mSonar;
SDistanceSensorData mLastDistanceData;
bool mIsInit;
};
