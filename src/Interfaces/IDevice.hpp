#pragma once

#include <Arduino.h>

struct IDeviceParams
{
  virtual ~IDeviceParams() {};

  virtual String getType() const = 0;
};

class IDevice
{
public:
	virtual ~IDevice() {};

	virtual void startDevice(const IDeviceParams & params) = 0;

	virtual void stopDevice() = 0;

	virtual bool isDeviceStarting() = 0;	
};
