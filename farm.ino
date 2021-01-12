#include "src/COneWireTemperatureSensor.hpp"
#include "src/CDiscreteDevice.hpp"
#include "src/CDiscreteSensor.hpp"
#include "src/CHCDistanceSensor.hpp"
#include "src/CWorker.hpp"

#define LIGHT 5

#define VENTILATION 4

#define ONE_WIRE_BUS 10

#define LEFT_BUTTON 0

#define RUGHT_BUTTON 2

#define NON_INTERRUPTS 20

#define PIN_ECHO 14
#define PIN_TRIG 12

CHCDistanceSensor* DistanceSensor = nullptr;
SHCDistanceSensorParams* DistanceSensorParams = nullptr;

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

unsigned long lastInterruptRightButton;

ICACHE_RAM_ATTR void rightButtonHanling()
{
  if(millis() - lastInterruptRightButton > NON_INTERRUPTS) // we set a 20ms no-interrupts window
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

    lastInterruptRightButton = millis();
  }
}

unsigned long lastInterruptLeftButton;

ICACHE_RAM_ATTR void leftButtonHanling()
{
  if(millis() - lastInterruptLeftButton > NON_INTERRUPTS) // we set a 20ms no-interrupts window
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

    lastInterruptLeftButton = millis();
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

    DistanceSensorParams = new SHCDistanceSensorParams();
    DistanceSensorParams->mEchoPin = PIN_ECHO;
    DistanceSensorParams->mTrigPin = PIN_TRIG;

    DistanceSensor = new CHCDistanceSensor(*DistanceSensorParams);

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
}

void loop() {

  if(nullptr != OneWireTemp)
  {
    delay(3000);

    const STemperatureSensorData & dataT = (const STemperatureSensorData &)OneWireTemp->getData();

    Serial.print("Temperature: ");
    Serial.print(dataT.mTemperature);
    Serial.println();

    const SDistanceSensorData & dataD = (const SDistanceSensorData &)DistanceSensor->getData();

    Serial.print("Distance: ");
    Serial.print(dataD.mDistance);
    Serial.println("cm");
  }
}
