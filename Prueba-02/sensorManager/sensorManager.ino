#include <SoftwareSerial.h>
#include "Sensor.h"

Sensor sensor(55); // create the sensor with the motor address

void setup(){

    //every function return an array of parammeters in bytes
    //if the parameter is longer than  byte it should be converted to int or long
    //using the next bytes
    sensor.getRealTime();
    sensor.getHistory();
    sensor.clearHistory();
}

void loop(){

}