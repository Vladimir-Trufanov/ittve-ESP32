/** Arduino, ESP32, C/C++ ************************************** ChipWiFi.h ***
 * 
 *                        Обеспечить сканирование и ведение локальной сети WiFi
 *                                               и собственной сети контроллера 
 *                                                     
 * v1.0.0, 19.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 18.03.2026
**/

#pragma once     

#include "WiFi.h"

void ScanWiFi(); 


 void ViewWiFi() 
{
 // Переведите Wi-Fi в режим станции и отключитесь от точки доступа, если она была подключена ранее.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
 


  //WiFi.STA.begin();
  delay(100);
  Serial.println("Setup done");

  Serial.println("-------------------------------------");
  Serial.println("Default wifi band mode scan:");
  Serial.println("-------------------------------------");
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 2)
  WiFi.setBandMode(WIFI_BAND_MODE_AUTO);
#endif
  ScanWiFi();
#if CONFIG_SOC_WIFI_SUPPORT_5G
  // Wait a bit before scanning again.
  delay(1000);
  Serial.println("-------------------------------------");
  Serial.println("2.4 Ghz wifi band mode scan:");
  Serial.println("-------------------------------------");
  WiFi.setBandMode(WIFI_BAND_MODE_2G_ONLY);
  ScanWiFi();
  // Wait a bit before scanning again.
  delay(1000);
  Serial.println("-------------------------------------");
  Serial.println("5 Ghz wifi band mode scan:");
  Serial.println("-------------------------------------");
  WiFi.setBandMode(WIFI_BAND_MODE_5G_ONLY);
  ScanWiFi();
#endif
  // Wait a bit before scanning again.
}

void ScanWiFi() 
{
  Serial.println("Scan start");
  // WiFi.scanNetworks will return the number of networks found.
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i)) {
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
  }

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();
  Serial.println("-------------------------------------");
}

// ****************************************************************************
// *              Показать состояние памяти с заданным префиксом              *
// ****************************************************************************


// ****************************************************************************
// *   Инициируем работу контроллера, как станции WiFi и с собственной сетью  *
// ****************************************************************************
void InitWiFi(const char* ssid, const char* password)
{

   // Enable Station Interface

  WiFi.mode(WIFI_MODE_APSTA);

  // char* soft_apssid = soft_ap_ssid;      // не более 10 символов, латиница
  WiFi.softAP("softapssid", "softapssid");
  // Подключаемся к WiFi
  //const char* ssid     = "TP-Link_B394";
  //const char* password = "18009217";

  WiFi.begin(ssid,password);
  WiFi.setSleep(false);
  Serial.print("Подключение к WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");








  /*
  //WiFi.mode(WIFI_MODE_APSTA);
  //char* soft_apssid = soft_ap_ssid;      // не более 10 символов, латиница
  //WiFi.softAP(soft_apssid, soft_apssid);
  
  
  //ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP("ESPWebServer", "12345678");    //Password length minimum 8 char

  //Comment below code if you are using Access point only
  //ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  //WiFi.begin(ssid, password);

  
   
  // Подключаемся к WiFi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.print("Подключение к WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
  // Заполняем параметры для передачи в html через status_handler 
  // [Вывод IPAddress в HTML](https://arduino.ru/forum/programmirovanie/vyvod-ipaddress-v-html)
  String(ssid).toCharArray(hssid,String(ssid).length()+1); 
  String ipaddr=WiFi.localIP().toString(); ipaddr.toCharArray(hlocalIP,ipaddr.length()+1); 
  ipaddr=WiFi.softAPIP().toString(); ipaddr.toCharArray(hsoftAPIP,ipaddr.length()+1); 
  */
}

// ************************************************************* ChipWiFi.h ***
