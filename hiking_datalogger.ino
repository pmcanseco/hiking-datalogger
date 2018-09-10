#include <SPI.h>
#include <epd1in54.h>
#include <epdpaint.h>
#include "GpsDevice.h"
#include "EpaperDevice.h"
#include "BMP180Device.h"

String classname = "[Main        ]";

GpsDevice* gpsDevice;
EpaperDevice* epaperDevice;
BMP180Device* bmp180Device;

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
  bmp180Device = new BMP180Device();

  // initialize me last so all my pointers are ready for when i need them.
  epaperDevice = new EpaperDevice(*gpsDevice, *bmp180Device);
  epaperDevice->init();
  
  logmsg("setup complete.");
}

void loop() {
  // put your main code here, to run repeatedly:

  gpsDevice->getRawGpsDataFromSerial();
  bmp180Device->getData();

  epaperDevice->fetchLon();
  epaperDevice->fetchLat();
  epaperDevice->fetchAlt();
  epaperDevice->fetchTmp();
  epaperDevice->fetchPrs();
  epaperDevice->drawData();

  delay(1000);
}
