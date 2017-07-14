/*

    Copyright (C) 2017 Libelium Comunicaciones Distribuidas S.L.
   http://www.libelium.com

    By using it you accept the MySignals Terms and Conditions.
    You can find them at: http://libelium.com/legal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Version:           0.1
    Design:            David Gascon
    Implementation:    Luis Martin / Victor Boria
*/

#include <MySignals.h>
#include "Wire.h"
#include "SPI.h" 

int valuePulse;
int valueSPO2;
uint8_t pulsioximeter_state = 0;

void setup() 
{

  Serial.begin(19200);
  MySignals.begin();
  
  MySignals.initSensorUART();
  MySignals.enableSensorUART(PULSIOXIMETER);
}

void loop() 
{

  // First way of getting sensor data
  MySignals.enableSensorUART(PULSIOXIMETER);
  Serial.println();
  pulsioximeter_state = MySignals.getPulsioximeter();
  if (pulsioximeter_state == 1)
  {
    Serial.print(F("Pulse:"));
    Serial.print(MySignals.pulsioximeterData.BPM);
    Serial.print(F("bpm / SPO2:"));
    Serial.print(MySignals.pulsioximeterData.O2);
    Serial.println(F("%"));
  }
  else if (pulsioximeter_state == 2)
  {
    Serial.println(F("Not valid data"));
  }
  else
  {
    Serial.println(F("No available data"));
  }
  MySignals.disableSensorUART();

  delay(2000);


  // Second way of getting sensor data
  MySignals.enableSensorUART(PULSIOXIMETER);
  valuePulse = MySignals.getPulsioximeter(PULSE);
  Serial.println();
  if (valuePulse == 2)
  {
    Serial.println(F("Not valid Pulse data"));
  }
  else if (valuePulse == 0)
  {
    Serial.println(F("No available data"));
  }
  else
  {
    Serial.print(F("Pulse:"));
    Serial.print(valuePulse);
    Serial.println(F("bpm"));
  }
  MySignals.disableSensorUART();

  
  delay(2000);


  MySignals.enableSensorUART(PULSIOXIMETER);
  valueSPO2 = MySignals.getPulsioximeter(SPO2);
  if (valueSPO2 == 2)
  {
    Serial.println(F("Not valid SPO2 data"));
  }
  else if (valueSPO2 == 0)
  {
    Serial.println(F("No available data"));
  }
  else
  {
    Serial.print(F("SPO2:"));
    Serial.print(valueSPO2);
    Serial.println(F("%"));
  }
  MySignals.disableSensorUART();


  delay(2000);
  
}





