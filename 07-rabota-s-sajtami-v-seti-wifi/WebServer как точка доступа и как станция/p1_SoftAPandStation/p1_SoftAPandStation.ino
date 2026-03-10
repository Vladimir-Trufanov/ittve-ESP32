// p1_SoftAPandStation.ino
//
// [ESP32 Arduino: Soft AP and Station modes](https://techtutorialsx.wordpress.com/2021/01/04/esp32-soft-ap-and-station-modes/)


#include <WiFi.h>

// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";

const char* wifi_network_ssid     = "TP-Link_B394";
const char* wifi_network_password = "18009217";
 
const char *soft_ap_ssid = "SoftAPand";  // p1_SoftAPandStation
const char *soft_ap_password = "testpassword";

void setup() 
{
 
  Serial.begin(115200);
   
  WiFi.mode(WIFI_MODE_APSTA);
 
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
  WiFi.begin(wifi_network_ssid, wifi_network_password);
 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());
 
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());
     
}

void loop() {}