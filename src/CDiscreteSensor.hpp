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

  uint8_t mDiscreteSensorPin;
  bool mIsUseInterrupts;
  int mInterruptType;
  std::function<void(void)> mFunc;
};

class CDiscreteSensor : public ISensor
{
public:
	CDiscreteSensor(SDiscreteSensorParams & discreteSensorParams) : ISensor()
	, mParams(discreteSensorParams)
  , mLastSensorData()
	, mIsInit(true)
  , mLastInterruptTime(0)
	{
    pinMode(mParams.mDiscreteSensorPin, INPUT);

    Serial.print(mParams.mDiscreteSensorPin);
    Serial.print(mParams.mIsUseInterrupts);

    if(true == mParams.mIsUseInterrupts)
    {        
      attachInterrupt(digitalPinToInterrupt(mParams.mDiscreteSensorPin), std::bind(&CDiscreteSensor::interruptHandler, this), mParams.mInterruptType);//
    }
	}

	~CDiscreteSensor()
	{
      if(true == mParams.mIsUseInterrupts)
      {
        detachInterrupt(digitalPinToInterrupt(mParams.mDiscreteSensorPin));
      }
	}

  friend void attachInterrupt(uint8_t pin, std::function<void(void)> intRoutine, int mode);

	virtual bool isInit() const
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

ICACHE_RAM_ATTR void interruptHandler()
{
  const unsigned long mNoInterruptsDelay = 20;

  if(nullptr != mParams.mFunc)
  { 
    if(millis() - mLastInterruptTime > mNoInterruptsDelay) // we set a 20ms no-interrupts window
    {
      mParams.mFunc();

      mLastInterruptTime = millis();
    }
  }
}

SDiscreteSensorParams mParams;
SDiscreteSensorData mLastSensorData;
bool mIsInit;

unsigned long mLastInterruptTime;
};
