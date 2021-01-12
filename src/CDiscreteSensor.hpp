#pragma once

#include "Interfaces/ISensor.hpp"

struct SDiscreteSensorData : public ISensorData
{
  SDiscreteSensorData() : ISensorData()
  , mDiscreteSensorState()
  {}

  ~SDiscreteSensorData()
  {}

  SDiscreteSensorData(const SDiscreteSensorData& other) : ISensorData()
  , mDiscreteSensorState(other.mDiscreteSensorState)
  {}

  SDiscreteSensorData& operator=(const SDiscreteSensorData& other)
  {
    if (&other != this) 
    {
        mDiscreteSensorState = other.mDiscreteSensorState;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "SDiscreteSensorData";
  }

  uint8_t mDiscreteSensorState;
};

struct SDiscreteSensorParams : public ISensorParams
{
  SDiscreteSensorParams() : ISensorParams()
  , mDiscreteSensorPin()
  , mIsUseInterrupts(false)
  , mInterruptType()
  , mFunc(nullptr)
  {}

  ~SDiscreteSensorParams()
  {}

  SDiscreteSensorParams(const SDiscreteSensorParams& other) : ISensorParams()
  ,mDiscreteSensorPin(other.mDiscreteSensorPin)
  ,mIsUseInterrupts(other.mIsUseInterrupts)
  ,mInterruptType(other.mInterruptType)
  ,mFunc(other.mFunc)
  {}

  SDiscreteSensorParams& operator=(const SDiscreteSensorParams& other)
  {
    if (&other != this) 
    {      
  		mDiscreteSensorPin = other.mDiscreteSensorPin;
      mIsUseInterrupts = other.mIsUseInterrupts;
      mInterruptType = other.mInterruptType;
      mFunc = other.mFunc;
    }

    return *this;
  }

  virtual String getType() const
  {
  	return "SDiscreteSensorParams";
  }

  uint8_t mDiscreteSensorPin;
  bool mIsUseInterrupts;
  int mInterruptType;
  void (*mFunc)(void);
};

class CDiscreteSensor : public ISensor
{
public:
	CDiscreteSensor(ISensorParams & buttonParams) : ISensor()
	, mParams()
  , mLastSensorData()
	, mIsInit(false)
	{
	  if(true == buttonParams.getType().equals("SButtonParams"))
	  {    
	    mParams = (const SDiscreteSensorParams&)(buttonParams);

	    pinMode(mParams.mDiscreteSensorPin, INPUT);

      Serial.print(mParams.mDiscreteSensorPin);
      Serial.print(mParams.mIsUseInterrupts);

      if(true == mParams.mIsUseInterrupts)
      {        
        attachInterrupt(digitalPinToInterrupt(mParams.mDiscreteSensorPin), mParams.mFunc, mParams.mInterruptType);
      }

	    mIsInit = true;
	  }
	}

	~CDiscreteSensor()
	{
      if(true == mParams.mIsUseInterrupts)
      {
        detachInterrupt(digitalPinToInterrupt(mParams.mDiscreteSensorPin));
      }
	}

	virtual bool isInit()
	{
		return mIsInit;
	}
	
	virtual const ISensorData & getData()
	{
		if(true == mIsInit)
		{
			mLastSensorData.mDiscreteSensorState = digitalRead(mParams.mDiscreteSensorPin);
		}

		return (const ISensorData &)(mLastSensorData);
	}

private:

SDiscreteSensorParams mParams;
SDiscreteSensorData mLastSensorData;
bool mIsInit;
};
