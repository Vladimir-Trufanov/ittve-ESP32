/*
 *  This sketch demonstrates how to scan WiFi networks. For chips that support 5GHz band, separate scans are done for all bands.
 *  The API is based on the Arduino WiFi Shield library, but has significant changes as newer WiFi functions are supported.
 *  E.g. the return value of `encryptionType()` different because more modern encryption is supported.
 */
#include "WiFi.h"
#include "trass.h"

// Определяем параметры сетей и подключаем библиотеку 
// ведения локальной и собственной сети WiFi 
#include "DefWiFi.h"

void setup() 
{
  Serial.begin(115200);
  delay(5000);

  bool isWiFi;
  for (int ii = 0; ii < 10; ii++) 
  {
    Serial.print(ii); Serial.print(": ");
    saymem("До ViewWiFi");
    isWiFi=ViewWiFi(sarr,narr);
    if (isWiFi) 
    {
      Serial.print("Будем подключать сеть: ");
      Serial.print(essid); Serial.print(" => "); Serial.print(epassword); Serial.print(" = ");  Serial.println(eRSSI);
    }
    else Serial.println("Ошибка, сеть НЕ найдена!");
    Serial.print(ii); Serial.print(": ");
    saymem("После ViewWiFi");
    Serial.print(ii); Serial.print(": ");
    saymem("До InitWiFi");
    InitWiFi(essid,epassword);
    Serial.print(ii); Serial.print(": ");
    saymem("После InitWiFi");
  }
}

void loop() 
{
  delay(100);
}
