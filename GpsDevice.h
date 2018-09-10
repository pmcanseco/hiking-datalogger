#ifndef GPSDEVICE_H
#define GPSDEVICE_H

#include <SoftwareSerial.h>

class GpsDevice {
public:
  GpsDevice();
  ~GpsDevice();
  void getRawGpsDataFromSerial();
  
  // gps getters
  String getLatitude();
  String getLongitude();
  String getAltitudeFeet();
  bool getGpsFixState(); // true=gps fixed, false=gps not fixed

private:
  const String classname = "[GpsDevice   ]";
  const int gpsRxPin = 10;
  const int gpsTxPin = 11;
  SoftwareSerial gpsSerial;
  static const int sentenceSize = 80;
  char sentence[sentenceSize];

  void parseReceivedGpsData();
  void getField(char* buffer, int index);
  void logmsg(String msg);
  void logGpsString(String msg);

  // gps field received state
  bool rmcReceived;
  bool ggaReceived;
  
  // gps fields
  double lat;                         // degrees minutes seconds
  String latDir;                      // N/S
  double lon;                         // degrees minutes seconds
  String lonDir;                      // E/W
  double altitudeMeters;              // in meters
  String theTimeUtc; 
  bool haveGpsFix;
  unsigned short numSatsInView;       // how many gps satellites we can see
  unsigned short numSatsBeingTracked; // how many gps satellites we're actually tracking
  
};
#endif
