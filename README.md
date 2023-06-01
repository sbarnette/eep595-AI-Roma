# eep595-AI-Roma

## Artificial_Nose.ino
This file contains the .ino sketch for the main project implementation. This includes sensor data collection, processing, classification, and print out of the best prediction via the serial output terminal. Additional debugging print outs are commented out but can be uncommented if the user desires more information about the readings. This file depends on the Wire, Circular Buffer, and Grove - Multichannel Gas Sensor libraries in addition to the compiled and quantized classifier library discussed below. All of these libraries can be acquired through the Arduino IDE Library Manager.

As a note, in the code, the include path for the Circular Buffer library is a specific folder on Sarah's PC. This is to work around an issue discovering the library as discussed in the library's GitHub [here](https://github.com/rlogiacco/CircularBuffer).

## Artificial_Nose_Collect_Data.ino
This file contains the .ino sketch for the data collection implementation. Edge Impulse can collect data directly from the Arduino Nano BLE 33 Sense using the Edge Impulse Data Forwarder. As long as the data is formatted in a specific way, a command line application forwards the data to the cloud at a certain frequency. We used this sketch to build our dataset from the ground up and collect it in Edge Impulse.

## ei-eep-595-ai-roma-arduino-1.0.49.zip
This .zip file contains the compiled library we include in Artificial_Nose.ino which is the output of the Edge Impulse model compression. This library has been integer quentized for optimal space and accuracy.

## ei-eep-t9t-ai-roma-classifier1.zip
This .zip file contains the files Edge Impulse uses for the classifier we wrote. The files within this .zip folder are necessary to edit the classifier locally if the user desires.
