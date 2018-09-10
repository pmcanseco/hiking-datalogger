#include "BMP180Device.h"
#include <Arduino.h>

#define LOGGING 1 // delete this line to turn logging off

BMP180Device::BMP180Device() :
  baselinePressure(0.0),
  altitudeMeters(0.0),
  pressureMb(0.0),
  temperatureC(0.0)
{
  if (sensor.begin())
  {
    logmsg("BMP180 init success");
    baselinePressure = getPressure();
    logmsg("baseline pressure: " + String(baselinePressure, 3) + " mb.");
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    logmsg("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }  
}
BMP180Device::~BMP180Device()
{}
void BMP180Device::logmsg(String msg)
{
  #ifdef LOGGING
  Serial.println(classname + " " + msg);
  #endif 
}
double BMP180Device::getPressure()
{
  char status;
  double P,p0;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = sensor.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = sensor.getTemperature(temperatureC);
    if (status != 0)
    {
      logmsg("temperature: " + String(temperatureC, 2) + " deg C,   " + String((9.0/5.0)*temperatureC+32.0, 2) + " deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.
      
      status = sensor.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = sensor.getPressure(P,temperatureC);
        if (status != 0)
        {
          // Print out the measurement:
          logmsg("absolute pressure: " + String(P,2) + " mb,   " + String(P*0.0295333727, 2) + " inHg");
          return(P);
        }
        else logmsg("error retrieving pressure measurement\n");
      }
      else logmsg("error starting pressure measurement\n");
    }
    else logmsg("error retrieving temperature measurement\n");
  }
  else logmsg("error starting temperature measurement\n");
}
void BMP180Device::getData()
{  
  // Get a new pressure reading:
  pressureMb = getPressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:
  double relativeAltitudeMeters = sensor.altitude(pressureMb,baselinePressure);
  altitudeMeters = sensor.altitude(pressureMb, CONST_AVG_PRS_MBAR_AT_SEA_LEVEL);

  logmsg("relative altitude:  " + String(relativeAltitudeMeters, 1) + " meters,   " + String(relativeAltitudeMeters*3.28084,0) + " feet.");
  logmsg("absolute altitude:  " + String(altitudeMeters, 1) + " meters,   " + String(altitudeMeters*3.28084,0) + " feet.");
}
String BMP180Device::getDisplayablePressureMb()
{
  String retval = String(pressureMb, 2);
  //logmsg("returning prs " + retval);
  return retval;
}
String BMP180Device::getDisplayableTemperatureF()
{
  String retval = String((9.0/5.0)*temperatureC+32.0, 2);
  //logmsg("returning tmp " + retval);
  return retval;
}
String BMP180Device::getDisplayableAltitudeFt()
{
  String retval = String(altitudeMeters*3.28084, 0);
  //logmsg("returning alt " + retval);
  return retval;
}

