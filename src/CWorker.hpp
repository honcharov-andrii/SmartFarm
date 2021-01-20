#pragma once

#include <Arduino.h>
#include <Ticker.h>

#include "Interfaces/IDevice.hpp"

class CWorker : private Ticker
{
public:
	CWorker(IDevice & device, IDeviceParams & params): Ticker()
	, mDevice(device)
  , mParams(params)
	{}

	~CWorker() 
	{
		stopWorking();
	};

	void startWorkingOnce(const float watchDogTime)
	{
    stopWorking();
    
    mWatchDogTime = watchDogTime;
    
		this->attach(watchDogTime, std::bind(&CWorker::tickerHandlerStartWorkingOnce, this));
   
		mDevice.startDevice(mParams);
	}

  void startWorkingCyclic(const float timeToWork, const float timeToStop)
  {
    stopWorking();
    
    mWatchDogTime = timeToWork;
    mTimeToStop = timeToStop;
    
    this->attach(timeToWork, std::bind(&CWorker::tickerHandlerStartWorkingCyclic, this));
    
    mDevice.startDevice(mParams);
  }

	void stopWorking()
	{
		this->detach();
   
	  if(true == mDevice.isDeviceStarted())
	  {
	    mDevice.stopDevice();
	  }
	}

  bool isDeviceWorking() const
  {
    return mDevice.isDeviceStarted();
  }

private:

	void tickerHandlerStartWorkingOnce()
	{
		stopWorking();
	}

  void tickerHandlerStartWorkingCyclic()
  {
    if(true == mDevice.isDeviceStarted())
    {
      mDevice.stopDevice();

      this->detach();

      this->attach(mTimeToStop, std::bind(&CWorker::tickerHandlerStartWorkingCyclic, this));
    }
    else
    {
      mDevice.startDevice(mParams);

      this->detach();

      this->attach(mWatchDogTime, std::bind(&CWorker::tickerHandlerStartWorkingCyclic, this));
    } 
  }

  Ticker mTicker;
  IDevice & mDevice;
  IDeviceParams & mParams;
  float mWatchDogTime;
  float mTimeToStop;
};
