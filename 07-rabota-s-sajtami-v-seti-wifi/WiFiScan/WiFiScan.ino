// Arduino, ESP32, C/C++ ************************************* WiFiScan.ino ***
//
//                                          Выполнить сканирование WiFi-сетей и 
//                                     подключить наиболее подходящую из списка
//
// v1.0.4, 23.03.2026                                 Автор:      Труфанов В.Е.
// Copyright © 2026 tve                               Дата создания: 18.03.2026

/** 
 * Arduino IDE 2.3.7 
 * Esp32 от Espressif Systems версии 3.3.5
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

#include "WiFi.h"
#include "trass.h"

// Подключаем библиотеку сканирования сетей WiFi, выбор и подключение 
// к локальной сети WiFi и собственной сети контроллера  (по умолчанию)
// #include "ChipWiFi.h"

// ИЛИ из вне библиотеки подключаем и переопределяем параметры сетей 
// для ведения локальной и собственной сети WiFi
#include "DefWiFi.h"

unsigned long currentMillis; // текущее время в миллисекундах  

void setup() 
{
  Serial.begin(115200);
  delay(5000);

  bool isViewWiFi=true;   // true - выполнять ViewWiFi, false - нет
  bool isInitWiFi=false;   // true - выполнять InitWiFi, false - нет

  bool isWiFi;
  for (int ii = 0; ii < 10; ii++) 
  {
    // Выполняем сканирование сетей WiFi и выбираем сеть для подключения
    if (isViewWiFi)
    {
      Serial.print(ii); Serial.print(": ");
      isWiFi=ViewWiFi(sarr,narr);
      if (isWiFi) 
      {
        Serial.print("Будем подключаться к сети: ");
        Serial.print(essid); Serial.print(" => "); Serial.print(epassword); Serial.print(" = ");  Serial.println(eRSSI);
      }
      else Serial.println("Ошибка, подходящая сеть НЕ найдена!");
      Serial.print(ii); Serial.print(": ");
    }
    // Подключаемся к локальной и собственной сетям WiFi
    if (isInitWiFi)
    {
      // Если сканирования не было, то будем подключаться к сети по умолчанию
      if (!isViewWiFi) 
      {
        strncpy(essid, "OPPO A9 2020", sizeof(essid) - 1); 
        strncpy(epassword, "b277a4ee84e8", sizeof(epassword) - 1); 
        Serial.print("Будем подключаться к сети: ");
        Serial.print(essid); Serial.print(" => "); Serial.print(epassword); Serial.print(" = ");  Serial.println(eRSSI);
      }
      // Иначе подключаемся к сети, выбранной в результате сканирования
      Serial.print(ii); Serial.print(": ");
      InitWiFi(essid,epassword);
      Serial.print(ii); Serial.print(": ");
    }
    // Фиксируем состояние памяти по завершении этапа цикла
    saymem("Этап цикла");
  }
  currentMillis = millis();
}

void loop() 
{
  // Показываем состояние памяти после каждых 2 минут
  if ((millis()-currentMillis) > 2000) 
  { 
    // Если прошло нужное количество времени  
    saymem("После 2 минут");
    currentMillis = millis();
  }  
  delay(100);
}

// *********************************************************** WiFiScan.ino ***
