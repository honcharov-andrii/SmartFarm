#pragma once

#include <Arduino.h>

struct ISensorParams
{
  virtual ~ISensorParams() {};

  virtual String getType() const = 0;
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

	virtual bool isInit() = 0;

	virtual const ISensorData & getData() = 0;	
};
