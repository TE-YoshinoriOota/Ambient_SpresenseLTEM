/*
 *  Spresense_position_to_ambient.ino 
 *  - Example for the Ambient library for Sony Spresense -
 *  Copyright 2020 Yoshinori Oota
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  This sketch connects to a website via LTE. Specifically,
 *  this example downloads the URL "http://www.arduino.cc/asciilogo.txt" and
 *  prints it to the Serial monitor.
 */

#include <Arduino.h>
#include <GNSS.h>
#include "Ambient_SpresenseLTEM.h"

/* Change the settings according to your sim card */
#define APN_NAME "your.apn.name"
#define APN_USRNAME "your.apn.usrname"
#define APN_PASSWD "your.apn.passwd"

/* Change the settings accroding to your ambient channel */
#define AMBI_CHANNEL 00000
#define AMBI_WRITEKEY "your.ambient.writekey"

/* Position data comes every 1 sec */
/* 10 sec interval */
#define UPDATE_POSITION_INTERVAL 10 

#define STRING_BUFFER_SIZE 128
char StringBuffer[STRING_BUFFER_SIZE];

SpGnss Gnss;
SpNavData NavData;

void setup() {
  Serial.begin(115200);

  /* Start up GNSS */
  int result = Gnss.begin();
  if (result != 0)  {
    Serial.println("Gnss begin error!!"); while(1);
  }
  
  Gnss.select(GPS); 
  Gnss.select(GLONASS); 
  Gnss.select(QZ_L1CA); 
  Gnss.select(QZ_L1S);
   
  bool ret = theAmbient.begin(APN_NAME ,APN_USRNAME ,APN_PASSWD);
  if (ret == false) {
    Serial.println("theAmbient begin failed");
    theAmbient.end(); while(1);
  }
  theAmbient.setupChannel(AMBI_CHANNEL, AMBI_WRITEKEY);

  result = Gnss.start(COLD_START);
  if (result != 0) {
    Serial.println("Gnss start error!!"); 
    theAmbient.end();
    while(1);
  }
}

void loop() {
  
  static int interval_cnt = 0; /* counter for the update interval */

  if (!Gnss.waitUpdate(-1))  {
    Serial.println("no update");
    return;
  }
  
  Gnss.getNavData(&NavData); /* get your position */

  /* print satellites count */
  snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSat:%2d, ", NavData.numSatellites);
  Serial.print(StringBuffer);

  /* print time */
  snprintf(StringBuffer, STRING_BUFFER_SIZE, "%04d/%02d/%02d %02d:%02d:%02d.%06d, "
         , NavData.time.year, NavData.time.month, NavData.time.day
         , NavData.time.hour, NavData.time.minute, NavData.time.sec
         , NavData.time.usec);
  Serial.print(StringBuffer);

  /* print position data */
  if (NavData.posFixMode == FixInvalid && NavData.posDataExist == 0) {
    Serial.print("No Position");
  } else {
    Serial.print("Lat = " + String(NavData.latitude, 6));
    Serial.print(" Lon = " + String(NavData.longitude, 6));
    Serial.print(" Alt = " + String(NavData.altitude, 6));
    if ((interval_cnt % UPDATE_POSITION_INTERVAL) == 0) {
      theAmbient.set( 1, NavData.altitude);   
      theAmbient.set( 9, NavData.latitude);
      theAmbient.set(10, NavData.longitude);
      theAmbient.send();
      interval_cnt = 0;
    }
  }
  Serial.println();
  ++interval_cnt;
  sleep(1); 
}
