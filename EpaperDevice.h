#ifndef EPAPERDEVICE_H
#define EPAPERDEVICE_H

#include <SPI.h>
#include <epd1in54.h>
#include <epdpaint.h>
#include "GpsDevice.h"


class EpaperDevice {
public:
  EpaperDevice(GpsDevice& gpsDevice);
  ~EpaperDevice();
  
  void init();
  
  void drawLabels();
  void drawUnits();
  void drawData(); // TODO remove this
  
  void drawLatData();
  void drawLonData();
  void drawAltData();
  void drawTmpData();
  void drawPrsData();
  void drawDirData();

  void fetchLat();
  void fetchLon();
  void fetchAlt();
  void fetchTmp();
  void fetchPrs();
  void fetchDir();

private:
  const String classname = "[EpaperDevice]";
  void logmsg(String msg);
  GpsDevice& gpsDevice;
  
  /**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
  unsigned char image[1024];
  Paint paint;  // width should be the multiple of 8 
  Epd epd;
  
  // LABELS
  const char latLabel[4] = {'L', 'A', 'T', '\0'}; 
  const char lonLabel[4] = {'L', 'O', 'N', '\0'}; 
  const char altLabel[5] = {'A', 'L', 'T', ':', '\0'}; 
  const char tmpLabel[5] = {'T', 'M', 'P', ':', '\0'}; 
  const char prsLabel[5] = {'P', 'R', 'S', ':', '\0'}; 
  const char dirLabel[5] = {'D', 'I', 'R', ':', '\0'};
  
  // UNITS
  const char altUnit[3] = {'f', 't', '\0'};
  const char tmpUnit[2] = {'F', '\0'};
  const char prsUnit[3] = {'m', 'b', '\0'};

  // DATA
  char latData[10] = {'0', '0', '0', '0', '0', '.', '0', '0', '0', '\0'}; // ex. 3000.452 N
  char lonData[10] = {'0', '0', '0', '0', '0', '.', '0', '0', '0', '\0'}; // ex. 10000.719 W
  char altData[7]  = {'0', '0', ',', '0', '0', '0', '\0'};                // ex. 14115 ft.
  char tmpData[6]  = {'0', '0', '0', '.', '0', '\0'};                     // ex. 101.2 F
  char prs_data[7] = {'0', '0', '0', '.', '0', '0', '\0'};                // ex. 847.18 mb
  char dir_data[4] = {'W', 'N', 'W', '\0'};                               // ex, ESE 'east south east'
};
#endif
