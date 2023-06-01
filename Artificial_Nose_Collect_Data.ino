#include <Arduino.h>
#include <Multichannel_Gas_GMXXX.h>
#include <Wire.h>
GAS_GMXXX<TwoWire>* gas = new GAS_GMXXX<TwoWire>();

typedef struct SENSOR_INFO
{
  char* name;
  char* unit;
  std::function<uint32_t()> readFn;
  uint32_t last_val;
} SENSOR_INFO;

SENSOR_INFO sensors[4] = {
  { "NO2", "ppm", std::bind(&GAS_GMXXX<TwoWire>::measure_NO2, gas), 0 },
  { "CO", "ppm", std::bind(&GAS_GMXXX<TwoWire>::measure_CO, gas), 0 },
  { "C2H5OH", "ppm", std::bind(&GAS_GMXXX<TwoWire>::measure_C2H5OH, gas), 0 },
  { "VOC", "ppm", std::bind(&GAS_GMXXX<TwoWire>::measure_VOC, gas), 0 }
};
#define NB_SENSORS 4

void setup()
{
  Serial.begin(115200);
  Serial.print("Serial Open!");

  gas->begin(Wire, 0x08); // use the hardware I2C
}

void loop()
{
  uint32_t sensorVal = sensors[0].readFn();
  if (sensorVal > 999) {
    sensorVal = 999;
  }
  Serial.print(sensorVal);
  Serial.print(", ");
  sensorVal = sensors[1].readFn();
  if (sensorVal > 999) {
    sensorVal = 999;
  }
  Serial.print(sensorVal);
  Serial.print(", ");
  sensorVal = sensors[2].readFn();
  if (sensorVal > 999) {
    sensorVal = 999;
  }
  Serial.print(sensorVal);
  Serial.print(", ");
  sensorVal = sensors[3].readFn();
  if (sensorVal > 999) {
    sensorVal = 999;
  }
  Serial.println(sensorVal);
}
