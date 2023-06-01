#include <Arduino.h>
#include "C:\Users\sastorer\OneDrive - Microsoft\Documents\Arduino\libraries\CircularBuffer-1.3.3\CircularBuffer.h"
#include <EEP_595_AI_Nose_inferencing.h>

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

char title_text[20] = "";

int latest_inference_idx = -1;
float latest_inference_confidence_level = -1.;

// Allocate a buffer for the values we'll read from the gas sensor
CircularBuffer<float, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE> buffer;

uint64_t next_sampling_tick = micros();

void setup()
{
  Serial.begin(115200);
  Serial.print("Serial Open!");

  gas->begin(Wire, 0x08); // use the hardware I2C
}

void loop()
{
  uint64_t new_sampling_tick = -1;
  if (micros() > next_sampling_tick) {
    new_sampling_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);
    next_sampling_tick = new_sampling_tick;
  }
  for (int i = NB_SENSORS - 1; i >= 0; i--) {
    uint32_t sensorVal = sensors[i].readFn();
    if (sensorVal > 999) {
      sensorVal = 999;
    }
    sensors[i].last_val = sensorVal;
    if (new_sampling_tick != -1) {
      buffer.unshift(sensorVal);
    }
  }

  // INFERENCE
  if (!buffer.isFull()) {
    Serial.print("Need more samples to start infering.\n");
  } else {
    // Turn the raw buffer into a signal which we can then classify
    float buffer2[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

    for (int i = 0; i < buffer.size(); i++) {
      buffer2[i] = buffer[i];
    }

    signal_t signal;
    int err = numpy::signal_from_buffer(
      buffer2, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
      Serial.print("Failed to create signal from buffer ");
      Serial.print(err);
      Serial.println();
      return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    Serial.println("Running Classifier");

    err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
      Serial.print("ERR: Failed to run classifier");
      Serial.print(err);
      Serial.println();
      return;
    }

    // print the predictions
    size_t best_prediction = 0;
    /*
    Serial.print("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d "
              "ms.): \n",
              result.timing.dsp,
              result.timing.classification,
              result.timing.anomaly);
    */

    //Serial.println("Calculating Best Prediction");

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
      if (result.classification[ix].value >=
          result.classification[best_prediction].value) {
        best_prediction = ix;
      }

      /*
      Serial.print("Label: ");
      Serial.print(result.classification[ix].label);
      Serial.println();
      Serial.print("Value: ");
      Serial.print(result.classification[ix].value);
      Serial.println();
      */
    }

  /*
  #if EI_CLASSIFIER_HAS_ANOMALY == 1
      sprintf(lineBuffer, "    anomaly score: %.3f\n", result.anomaly);
  #endif
  */

  /*
    sprintf(title_text,
            "%s (%d%%)",
            result.classification[best_prediction].label,
            (int)(result.classification[best_prediction].value * 100));
  */
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("Best prediction: ");
    Serial.print(result.classification[best_prediction].label);
    Serial.print(" (");
    Serial.print((int)(result.classification[best_prediction].value * 100));
    Serial.println("%)");

    latest_inference_idx = best_prediction;
    latest_inference_confidence_level =
      result.classification[best_prediction].value;
  }
}
