/*
  В этом примере показано, как использовать библиотеку Wi-Fi для сканирования 
  доступных сетей Wi-Fi в асинхронном режиме и печати результатов.

  2026-03-19 На сканировании большое потребление энергии, 
  бывает ошибка "E BOD: Brownout detector was triggered"
*/
#include "WiFi.h"

//void startWiFiScan() 
//{
//  Serial.println("Начало сканирования");
  // WiFi.scanNetworks немедленно вернется в асинхронный режим.
//  WiFi.scanNetworks(true);  // 'true' включает асинхронный режим
//}

void printScannedNetworks(uint16_t networksFound) 
{
  if (networksFound == 0) 
  {
    Serial.println("Не найдено ни одной сети Wi-Fi");
  } 
  else 
  {
    Serial.println("\nСканирование завершено");
    Serial.print(networksFound); Serial.println(" найденных сетей");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < networksFound; ++i) 
    {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i)) 
      {
        case WIFI_AUTH_OPEN:            Serial.print("open"); break;
        case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
        default:                        Serial.print("unknown");
      }
      Serial.println();
      delay(10);
    }
    Serial.println("");
    // Удалите результат сканирования, чтобы освободить память для приведенного ниже кода.
    WiFi.scanDelete();
  }
}

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  
  // Переведите Wi-Fi в режим станции и отключитесь от точки доступа, если она была подключена ранее.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  /*
  // Инициируем переменную статуса сканирования
  int16_t WiFiScanStatus = 0;
  // Делаем 5 попыток сканирования сетей
  bool isScan=false;
  for (int i = 0; i < 10; ++i) 
  {
    WiFi.scanNetworks(true);  // 'true' включает асинхронный режим
    WiFiScanStatus = WiFi.scanComplete();
    if (WiFiScanStatus < 0) 
    {
      Serial.println("Не удалось выполнить сканирование Wi-Fi. Запускаем снова.");
      delay(500);
    }
    else
    { 
      isScan=false; break;  
    }
  }
  if (isScan) printScannedNetworks(WiFiScanStatus);
  */

  
  //startWiFiScan();
  WiFi.scanNetworks(true);  // 'true' включает асинхронный режим
  
}

void loop() 
{
  // проверьте асинхронный процесс сканирования Wi-Fi
  int16_t WiFiScanStatus = WiFi.scanComplete();
  if (WiFiScanStatus < 0) 
  {  
    // он занят сканированием или получил сообщение об ошибке
    if (WiFiScanStatus == WIFI_SCAN_FAILED) 
    {
      Serial.println("Не удалось выполнить сканирование Wi-Fi. Запускаем снова.");
      WiFi.scanNetworks(true);  // 'true' включает асинхронный режим
      //startWiFiScan();
    }
    // другой вариант - статус WIFI_SCAN_RUNNING - просто подождите.
    Serial.println("Делаем паузу.");
    delay(100);
  } 
  else 
  {  
    // Найдено ноль или более беспроводных сетей
    printScannedNetworks(WiFiScanStatus);
    Serial.println("Делаем сканирование еще раз.");
    WiFi.scanNetworks(true);  // 'true' включает асинхронный режим
    //startWiFiScan();  // start over...
    Serial.println("Второй раз сделали.");
  }
  // Loop can do something else...
  delay(10250);
  //Serial.println("Loop running...");
}
