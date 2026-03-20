/*
 *  This sketch demonstrates how to scan WiFi networks. For chips that support 5GHz band, separate scans are done for all bands.
 *  The API is based on the Arduino WiFi Shield library, but has significant changes as newer WiFi functions are supported.
 *  E.g. the return value of `encryptionType()` different because more modern encryption is supported.
 */
#include "WiFi.h"
#include "ChipWiFi.h"
#include "trass.h"

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  
  saymem("До ViewWiFi"); 
  bool isWiFi=ViewWiFi();
  if (isWiFi) 
  {
    Serial.print("Будем подключать сеть: ");
    Serial.print(essid); Serial.print(" => "); Serial.print(epassword); Serial.print(" = ");  Serial.println(eRSSI);
  }
  else Serial.println("Ошибка, сеть НЕ найдена!");
  saymem("До InitWiFi"); 
  InitWiFi(essid,epassword);
  saymem("В конце Setup"); 
}

void loop() 
{
  delay(100);
}
