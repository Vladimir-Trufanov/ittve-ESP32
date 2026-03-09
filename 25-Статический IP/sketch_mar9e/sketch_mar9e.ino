
/*
13:05:05.490 -> Connecting to TP-Link_B394
13:05:06.540 -> ..
13:05:07.559 -> Connected..!
13:05:07.559 -> Current ESP32 IP: 192.168.0.105
13:05:07.559 -> Gateway (router) IP: 192.168.0.1
13:05:07.559 -> Subnet Mask: 255.255.255.0
13:05:07.559 -> Primary DNS: 192.168.0.1
13:05:07.559 -> Secondary DNS: 0.0.0.0
*/

#include <WiFi.h>

// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Static IP configuration
IPAddress staticIP(192, 168, 0, 200); // ESP32 static IP
IPAddress gateway(192, 168, 0, 1);    // IP Address of your network gateway (router)
IPAddress subnet(255, 255, 255, 0);   // Subnet mask
IPAddress primaryDNS(192, 168, 0, 1); // Primary DNS (optional)
IPAddress secondaryDNS(0, 0, 0, 0);   // Secondary DNS (optional)

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Configuring static IP
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address to Serial Monitor
}

void loop() {
  // Nothing to do here
}
