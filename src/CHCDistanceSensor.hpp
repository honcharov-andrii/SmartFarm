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

  uint8_t mEchoPin;
  uint8_t mTrigPin; 
};

class CHCDistanceSensor : public ISensor
{
public:
	CHCDistanceSensor(SHCDistanceSensorParams & sensorParams) : ISensor()
	, mParams(sensorParams)
  , mSonar(nullptr)
	, mIsInit(true)
	{
    mParams = (const SHCDistanceSensorParams&)(sensorParams);
    mSonar = new Ultrasonic(mParams.mTrigPin, mParams.mEchoPin);
	}

	~CHCDistanceSensor()
	{
    if(nullptr != mSonar)
    {
      delete mSonar;
    }
  }

	virtual bool isInit() const
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
