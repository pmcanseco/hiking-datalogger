#include "EpaperDevice.h"
#include <Arduino.h>

#define LOGGING 1 // delete this line to turn logging off

#define COLORED     0
#define UNCOLORED   1

EpaperDevice::EpaperDevice(GpsDevice& gpsDevice) :
  paint(image, 0, 0),
  gpsDevice(gpsDevice)
{ }
EpaperDevice::~EpaperDevice()
{ }
void EpaperDevice::init()
{
  logmsg("initializing screen...");
  if (epd.Init(lut_full_update) != 0) {
      logmsg("e-Paper init failed");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  paint.SetRotate(ROTATE_0);
  paint.SetWidth(200);
  paint.SetHeight(24);

  paint.Clear(COLORED);
  paint.DrawStringAt(30, 4, "HIKING", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, "data logger", &Font16, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  delay(1000);

  if (epd.Init(lut_partial_update) != 0) {
      logmsg("e-Paper init failed");
      return;
  }

  logmsg("blinking screen twice to clear away potential artifacts");
  epd.ClearFrameMemory(0x00);
  epd.DisplayFrame();
  epd.ClearFrameMemory(0x00);
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();

  // calling twice to set these on both eink framebuffers
  // the screen auto-toggles between each framebuffers with each call to epd.DisplayFrame()
  // we want the units and labels to be on both framebuffers so they show at all times.
  logmsg("drawing labels");
  drawLabels();
  drawLabels();
  logmsg("drawing units");
  drawUnits();
  drawUnits();
  logmsg("drawing data");
  drawData();
  drawData();
  logmsg("screen initialization complete.");
}
void EpaperDevice::drawUnits() 
{
  paint.SetWidth(28);
  paint.SetHeight(30); 
  paint.SetRotate(ROTATE_270);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, altUnit, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 60, 7, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, tmpUnit, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 80, 7, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, prsUnit, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 110, 7, paint.GetWidth(), paint.GetHeight());
  
  epd.DisplayFrame();  
}

void EpaperDevice::drawLabels() 
{
  paint.SetWidth(28);
  paint.SetHeight(45); 
  paint.SetRotate(ROTATE_270);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, latLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 15, 154, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, lonLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 35, 154, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(28);
  paint.SetHeight(55); 

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, altLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 60, 143, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, tmpLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 80, 143, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, prsLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 110, 143, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, dirLabel, &Font20, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 130, 143, paint.GetWidth(), paint.GetHeight());
  
  epd.DisplayFrame();  
}

void EpaperDevice::drawData()
{
  paint.SetWidth(22);
  paint.SetHeight(155); 
  paint.SetRotate(ROTATE_270);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, latData, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 15, 0, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, lonData, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 35, 0, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(28);
  paint.SetHeight(105);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, altData, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 60, 38, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, tmp_data, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 80, 38, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, prs_data, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 110, 38, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, dir_data, &Font24, COLORED);
  // y offset, x offset
  epd.SetFrameMemory(paint.GetImage(), 130, 38, paint.GetWidth(), paint.GetHeight());
  
  epd.DisplayFrame();
}
void EpaperDevice::fetchLat() {
  gpsDevice.getLatitude().toCharArray(latData, 10);
  logmsg("Fetched latitude of " + String(latData) + " from gps.");
}
void EpaperDevice::fetchLon() {
  gpsDevice.getLongitude().toCharArray(lonData, 10);
  logmsg("Fetched longitude of " + String(lonData) + " from gps.");
}
void EpaperDevice::fetchAlt() {
  gpsDevice.getAltitudeFeet().toCharArray(altData, 6);
  logmsg("Fetched altitude of " + String(altData) + " ft from gps.");
}
void EpaperDevice::fetchTmp() {
  
}
void EpaperDevice::fetchPrs() {
  
}
void EpaperDevice::fetchDir() {
  
}
void EpaperDevice::logmsg(String msg) {
  #ifdef LOGGING
  Serial.println(classname + " " + msg);
  #endif
}
