#include <SPI.h>
#include <epd1in54.h>
#include <epdpaint.h>
#include "GpsDevice.h"
#include "EpaperDevice.h"

String classname = "[Main        ]";

GpsDevice* gpsDevice;
EpaperDevice* epaperDevice;

void logmsg(String msg) 
{
  Serial.println(classname + " " + msg);
}

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  Serial.println();
  logmsg("HIKING DATA LOGGER");
   
  gpsDevice = new GpsDevice();
  epaperDevice = new EpaperDevice(*gpsDevice);

  epaperDevice->init();
  
  logmsg("setup complete.");
}

void loop() {
  // put your main code here, to run repeatedly:

  gpsDevice->getRawGpsDataFromSerial();

  epaperDevice->fetchLon();
  epaperDevice->fetchLat();
  epaperDevice->fetchAlt();
  epaperDevice->drawData();

  delay(1000);
}
