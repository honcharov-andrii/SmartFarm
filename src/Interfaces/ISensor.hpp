#pragma once

#include <Arduino.h>

struct ISensorParams
{
  virtual ~ISensorParams() {};
};

struct ISensorData
{
  virtual ~ISensorData() {};

  virtual String getType() const = 0;
};

class ISensor
{
public:
	
	virtual ~ISensor() {};

	virtual bool isInit() const = 0;

	virtual const ISensorData & getData() = 0;	
};
