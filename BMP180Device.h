#ifndef BMP180DEVICE_H
#define BMP180DEVICE_H

#include <Arduino.h>
#include <SFE_BMP180.h>
#include <Wire.h>

/* COPY-PASTE Documentation from Example Sketch:
 * This sketch shows how to use the Bosch BMP180 pressure sensor
 * as an altimiter. https://www.sparkfun.com/products/11824
 * 
 * Like most pressure sensors, the BMP180 measures absolute pressure.
 * Since absolute pressure varies with altitude, you can use the pressure
 * to determine your altitude.
 * 
 * Because pressure also varies with weather, you must first take a pressure
 * reading at a known baseline altitude. Then you can measure variations
 * from that pressure    
 * Hardware connections:
 *  - (GND) to GND
 *  + (VDD) to 3.3V   
 *  (WARNING: do not connect + to 5V or the sensor will be damaged!)    
 *  
 *  You will also need to connect the I2C pins (SCL and SDA) to your    
 *  Arduino. The pins are different on different Arduinos:    
 *  
 *  Any Arduino pins labeled:  SDA  SCL    
 *  Uno, Redboard, Pro:        A4   A5    
 *  Mega2560, Due:             20   21    
 *  Leonardo:                   2    3
 */

class BMP180Device {
public:
  BMP180Device();
  ~BMP180Device();
  void getData();

  String getDisplayablePressureMb();
  String getDisplayableTemperatureF();
  String getDisplayableAltitudeFt();
 
private:
  const String classname = "[BMP180Device]";
  
  SFE_BMP180 sensor;
  double baselinePressure;
  double altitudeMeters;
  double pressureMb;
  double temperatureC;

  void logmsg(String msg); 
  double getPressure();
};
#endif
