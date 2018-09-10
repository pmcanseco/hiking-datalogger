#include "GpsDevice.h"
#include <Arduino.h>

//#define LOGGING 1 // delete this line to turn logging off
//#define LOGRAWGPSSTRINGS 1 // delete this line to stop printing full GPS strings

GpsDevice::~GpsDevice() {
};

GpsDevice::GpsDevice() :
  gpsSerial(gpsRxPin, gpsTxPin),
  haveGpsFix(false),
  lat(0.0),               // degrees minutes seconds
  latDir(""),             // N/S
  lon(0.0),               // degrees minutes seconds
  lonDir(""),             // E/W
  altitudeMeters(0.0),    // in meters
  theTimeUtc(""), 
  numSatsInView(0),       // how many gps satellites we can see
  numSatsBeingTracked(0), // how many gps satellites we're actually tracking
  rmcReceived(false),
  ggaReceived(false)
{
  gpsSerial.begin(9600);
};

void GpsDevice::getRawGpsDataFromSerial() {
  logmsg("Reading GPS Serial...");
  static int i = 0;
  while (!(rmcReceived && ggaReceived))
  {
    while (gpsSerial.available())
    {
      char ch = gpsSerial.read();
      if (ch != '\n' && i < sentenceSize)
      {
        sentence[i] = ch;
        i++;
      }
      else
      {
       sentence[i] = '\0';
       i = 0;
       parseReceivedGpsData();
      }
    }
  }
  rmcReceived = false;
  ggaReceived = false;
  logmsg("---------------------------");
}
void GpsDevice::parseReceivedGpsData() {
  char field[20];
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    ////// LAT
    //String(latFloat, 4).toCharArray(gps_data_lat, 10);
    getField(field, 3);  // number
    lat = String(field).toDouble();
    getField(field, 4); // N/S
    latDir = field;
    logmsg("Lat: " + String(lat, 4) + latDir);

    ////// LON
    //String(lonFloat, 4).toCharArray(gps_data_lon, 10);
    getField(field, 5);  // number
    lon = String(field).toDouble();
    getField(field, 6);  // E/W
    lonDir = field;
    logmsg("Lon: " + String(lon, 4) + lonDir);

    ////// TIME
    getField(field, 1);
    theTimeUtc = field;
    logmsg("TIME: " + theTimeUtc);


    logmsg("Parsed RMC fields.");
    logGpsString(sentence);
    rmcReceived = true;
  }
  if (strcmp(field, "$GPGGA") == 0)
  {
    ////// FIX
    getField(field, 6); // 0=invalid 1=gpsfix ... etc.
    if (strcmp(field, "1") == 0)
    {
      haveGpsFix = true;
    }

    ////// NUM SATS TRACKED
    getField(field, 7); // number
    numSatsBeingTracked = String(field).toInt();
    logmsg("Satellites Tracked: " + String(numSatsBeingTracked));
    
    ////// ALTITUDE
    //String(altFloat, 0).toCharArray(alt_data, 6);
    getField(field, 9);  // number
    altitudeMeters = String(field).toDouble();
    float altitudeFeet = altitudeMeters * 3.28084; // convert to feet
    logmsg("Alt: " + String(altitudeFeet, 4) + "ft");
    // field 10 has a letter M for when you want meters directly
    getField(field, 10);
      

    logmsg("Parsed GGA fields.");
    logGpsString(sentence);
    ggaReceived = true;
  }
  if ( (strcmp(field, "$GPGSV") == 0) || (strcmp(field, "$GPGSA") == 0) )
  {
    logmsg("Printing GSV and GSA fields.");
    logGpsString(sentence);
    ggaReceived = true;
  }
}
void GpsDevice::getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 
String GpsDevice::getLatitude() {
  if(!haveGpsFix)
  {
    return " NOGPS";
  }
  return String(lat, 4);
}
String GpsDevice::getLongitude() {
  if(!haveGpsFix)
  {
    return " NOGPS";
  }
  return String(lon, 4);
}
String GpsDevice::getAltitudeFeet() {
  return String(altitudeMeters * 3.28084, 0);
}
bool GpsDevice::getGpsFixState() {
  return haveGpsFix;
}
void GpsDevice::logmsg(String msg) {
  #ifdef LOGGING
  Serial.println(classname + " " + msg);
  #endif
}
void GpsDevice::logGpsString(String msg) {
  #ifdef LOGRAWGPSSTRINGS
  Serial.println(classname + " RAW: " + msg);
  #endif
}


