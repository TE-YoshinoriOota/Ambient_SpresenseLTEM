/*
 *  Spresense LTE-M Ambient Library 
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
 */


#include "Ambient_SpresenseLTEM.h"

#define AMBI_DEBUG

LTE lteAccess;
LTEClient client;

const char * ambient_keys[] = {"\"d1\":\"", "\"d2\":\"", "\"d3\":\"", "\"d4\":\"", "\"d5\":\"", "\"d6\":\"", "\"d7\":\"", "\"d8\":\"", "\"lat\":\"", "\"lng\":\"", "\"created\":\""};

char server[] = "54.65.206.59";
char path[] = "/api/v2/channels/";
int port = 80; 

Ambient_SpresenseLTEM::Ambient_SpresenseLTEM() {
  mInitialized = false;
}

Ambient_SpresenseLTEM::~Ambient_SpresenseLTEM() {
}

bool Ambient_SpresenseLTEM::begin(String apn_name, String apn_usrname, String apn_passwd) {

  int count = 0;
  while (count < AMBIENT_MAX_RETRY) {
    if (lteAccess.begin() == LTE_SEARCHING) {
      if (lteAccess.attach(apn_name.c_str()
                         , apn_usrname.c_str()
                         , apn_passwd.c_str()) == LTE_READY) {
#ifdef AMBI_DEBUG
        Serial.println("attach succeeded.");
#endif
        return true;
      }
      Serial.println("An error occurred, shutdown and try again.");
      lteAccess.shutdown();
	  ++count;
      sleep(1);
    }
  }
  return false;

}

void Ambient_SpresenseLTEM::setupChannel(uint16_t channelId, String writeKey) {
  mChannelId = channelId;
  mWriteKey = writeKey;
  mInitialized = true;
  return;
}

bool Ambient_SpresenseLTEM::set(int field, String data) {
  --field;
  if (field < 0 || field >= AMBIENT_NUM_PARAMS) {
    return false;
  }
  if (data.length() > AMBIENT_DATA_SIZE) {
    return false;
  }
  mData[field].set = true;
  mData[field].item = data;

  return true;
}

bool Ambient_SpresenseLTEM::set(int field, double data) {
  return set(field, String(data, 12));
}

bool Ambient_SpresenseLTEM::set(int field, int data) {
  return set(field, String(data));
}

bool Ambient_SpresenseLTEM::clear(int field) {
  --field;
  if (field < 0 || field >= AMBIENT_NUM_PARAMS) {
    return false;
  }
  mData[field].set = false;
  mData[field].item = "";

  return true;
}

bool Ambient_SpresenseLTEM::clearAll() {

  for (int i = 0; i < AMBIENT_NUM_PARAMS; ++i) {
    mData[i].set = false;
    mData[i].item = "";
  }

  return true;
}


bool Ambient_SpresenseLTEM::send() {

  if (mInitialized == false) return false;
 

  /* Retrieve the file with the specified URL. */
  String post = "{\"writeKey\":\"" + mWriteKey + "\",";

  for (int i = 0; i < AMBIENT_NUM_PARAMS; ++i) {
    if (mData[i].set) {
      post += String(ambient_keys[i]);
      post += mData[i].item;
      post += "\",";
    }
  }
  post.setCharAt(post.length()-1,'}'); // to overwrite the last conmma 

  String ctype = String("application/json");

  String fullpath = path + String(mChannelId) + "/data";

  if (client.connect(server, port)) {

    client.println("POST " + fullpath + " HTTP/1.1");
    client.println("Host: " + String(server)); // + String(":") + String(port));
    client.println("Content-Length: " + String(post.length()));
    client.println("Content-Type: " + ctype);
    client.println("User-Agent: NuttX/6.xx.x (; http://www.nuttx.org/)");
    client.println("Connection: close");
    client.println();

#ifdef AMBI_DEBUG
    Serial.println();
    Serial.println("connected to ambidata.io");
    Serial.println("POST " + fullpath + " HTTP/1.1");
    Serial.println("Host: " + String(server)); // + String(":") + String(port));
    Serial.println("Content-Length: " + String(post.length()));
    Serial.println("Content-Type: " + ctype);
    Serial.println("User-Agent: NuttX/6.xx.x (; http://www.nuttx.org/)");
    Serial.println("Connection: close");
    Serial.println();
#endif

    delay(100);
    if(client.println(post) == 0){
      return false;
    }

#ifdef AMBI_DEBUG
    Serial.println(post);
#endif

  } else {
    Serial.println("Connection failed to ambidata.io");
    return false;
  }

  delay(100);

  // waiting for the response
  while (1) {
    if (int len = client.available()) {
      char buff[len + 1];
      client.read((uint8_t*)buff, len);
      Serial.print(buff);
    }

    if (!client.available() && !client.connected()) {
      Serial.println();
      Serial.println("disconnected");
      break;
    }
  }

  client.stop();

  return true;

}

void Ambient_SpresenseLTEM::end() {
  client.stop();
  lteAccess.shutdown();
}

Ambient_SpresenseLTEM theAmbient;
