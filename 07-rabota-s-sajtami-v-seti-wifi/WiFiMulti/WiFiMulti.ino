// [ESP32 WiFiMulti подключается к самой мощной сети Wi-Fi]
// (https://microcontrollerslab.com/esp32-wifimulti-connect-to-the-strongest-wifi-network/)

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

const uint32_t connectTimeoutMs = 10000;

void setup()
{
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  
  wifiMulti.addAP("TP-Link_B394",  "18009217");
  wifiMulti.addAP("tve-DESKTOP",   "Ue18-647");
  wifiMulti.addAP("OPPO A9 2020",  "b277a4ee84e8");
  wifiMulti.addAP("tve-MONOBLOCK", "Ue18-647");
  wifiMulti.addAP("linksystve",    "X93K6KQ6WF");
  wifiMulti.addAP("GoshaIMila",    "t1s2wde4bE");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) 
  {
      Serial.println("no networks found");
  } 
  else 
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) 
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println();
      delay(10);
    }
  }

  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("Wi-Fi CONNECTED!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop()
{
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) 
  {
    Serial.print("Connected with: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.RSSI());
  }
  else 
  {
    Serial.println("WiFi not connected!");
  }
  delay(60000);
}



/*
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(1000);
  }
}
*/
