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

#ifndef AMBIENT_SPRESENSE_LTEM_H
#define AMBIENT_SPRESENSE_LTEM_H

#include <Arduino.h>
#include <LTE.h>

#define AMBIENT_WRITEKEY_SIZE  18
#define AMBIENT_MAX_RETRY      10
#define AMBIENT_DATA_SIZE      24
#define AMBIENT_NUM_PARAMS     11


class Ambient_SpresenseLTEM {
public:
  Ambient_SpresenseLTEM();
  ~Ambient_SpresenseLTEM();
  bool begin(String apn_name, String apn_usr_name, String apn_passwd);
  void setupChannel(uint16_t channelId, String writeKey);
  bool set(int field, String data);
  bool set(int field, double data);
  bool set(int field, int data);
  bool clear(int field);
  bool clearAll();
  bool send();
  void end();
private:
  bool mInitialized;
  uint16_t mChannelId;
  String mWriteKey;

  struct {
    int set;
    String item;
  } mData[AMBIENT_NUM_PARAMS];
};

extern Ambient_SpresenseLTEM theAmbient;

#endif
