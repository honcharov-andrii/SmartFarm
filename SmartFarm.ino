#include "src/COneWireTemperatureSensor.hpp"
#include "src/CDiscreteDevice.hpp"
#include "src/CDiscreteSensor.hpp"
#include "src/CHCDistanceSensor.hpp"
#include "src/CWorker.hpp"

#define LIGHT 5

#define VENTILATION 16

#define ONE_WIRE_BUS 10

#define BARRIER_SENSOR 4

#define LEFT_BUTTON 0

#define RUGHT_BUTTON 2

OneWire OneWireBus(ONE_WIRE_BUS); 
DallasTemperature DallasSensors(&OneWireBus);
ISensor* OneWireTemp = nullptr;
SOneWireTemperatureSensorParams* OneWireTempParams = nullptr;

IDevice* Lighting = nullptr;
SDiscreteDeviceParams* LightingParams = nullptr;

IDevice* Ventilation = nullptr;
SDiscreteDeviceParams* VentilationParams = nullptr;

CWorker* LightingWorker = nullptr;

CWorker* VentilationWorker = nullptr;

SDiscreteSensorParams* RightButtonParams = nullptr;
CDiscreteSensor* RightButton = nullptr;

SDiscreteSensorParams* LeftButtonParams = nullptr;
CDiscreteSensor* LeftButton = nullptr;

SDiscreteSensorParams* BarrierSensorParams = nullptr;
ISensor* BarrierSensor = nullptr;

void rightButtonHanling()
{
  if(nullptr != LightingWorker)
  {
    if(true == LightingWorker->isDeviceWorking())
    {
      LightingWorker->stopWorking();
    }
    else
    {
      LightingWorker->startWorkingOnce(60);
    }
  }
}

void leftButtonHanling()
{
  if(nullptr != VentilationWorker)
  {
    if(true == VentilationWorker->isDeviceWorking())
    {
      VentilationWorker->stopWorking();
    }
    else
    {
      VentilationWorker->startWorkingOnce(60);
    }
  }
}

void setup() {
    Serial.begin(115200);
    Serial.println("");

    delay(3000);

    DeviceAddress insideThermometer;
    if (!DallasSensors.getAddress(insideThermometer, 0)) 
    {
      Serial.println("Unable to find address for Device 0");
    }

    OneWireTempParams = new SOneWireTemperatureSensorParams();
    OneWireTempParams->mSensors = DallasSensors;
    for(uint8_t i = 0; i < 8; ++i)
    {
      OneWireTempParams->mDeviceAddress[i] = insideThermometer[i];
    }

    OneWireTemp = new COneWireTemperatureSensor(*OneWireTempParams);

    LightingParams = new SDiscreteDeviceParams();
    LightingParams->mDiscreteDevicePin = LIGHT;

    VentilationParams = new SDiscreteDeviceParams();
    VentilationParams->mDiscreteDevicePin = VENTILATION;

    Lighting = new CDiscreteDevice();

    Ventilation = new CDiscreteDevice();

    LightingWorker = new CWorker(*Lighting, *LightingParams);

    VentilationWorker = new CWorker(*Ventilation, *VentilationParams);

    RightButtonParams = new SDiscreteSensorParams();
    RightButtonParams->mDiscreteSensorPin = RUGHT_BUTTON;
    RightButtonParams->mIsUseInterrupts = true;
    RightButtonParams->mInterruptType = RISING;
    RightButtonParams->mFunc = rightButtonHanling;

    RightButton = new CDiscreteSensor(*RightButtonParams);

    LeftButtonParams = new SDiscreteSensorParams();
    LeftButtonParams->mDiscreteSensorPin = LEFT_BUTTON;
    LeftButtonParams->mIsUseInterrupts = true;
    LeftButtonParams->mInterruptType = RISING;
    LeftButtonParams->mFunc = leftButtonHanling;

    LeftButton = new CDiscreteSensor(*LeftButtonParams);
    
    BarrierSensorParams = new SDiscreteSensorParams();
    BarrierSensorParams->mDiscreteSensorPin = BARRIER_SENSOR;
    BarrierSensorParams->mIsUseInterrupts = false;
    //BarrierSensorParams->mInterruptType = RISING;
    //BarrierSensorParams->mFunc = rightButtonHanling;

    BarrierSensor = new CDiscreteSensor(*BarrierSensorParams);
}

void loop() {

  if(nullptr != OneWireTemp && nullptr != BarrierSensor)
  {
    delay(3000);

    auto dataT = static_cast<const STemperatureSensorData &>(OneWireTemp->getData());

    Serial.print("Temperature: ");
    Serial.print(dataT.mTemperature);
    Serial.println("°C");

    auto dataB = static_cast<const SDiscreteSensorData &>(BarrierSensor->getData());

    Serial.print("Barrier: ");
    Serial.println(!static_cast<bool>(dataB.mDiscreteSensorState));
  }
}
