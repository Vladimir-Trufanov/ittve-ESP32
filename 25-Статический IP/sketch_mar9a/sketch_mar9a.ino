#include <WiFi.h>  

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Static IP configuration  
/*
IPAddress staticIP(192, 168, 1, 100);  
IPAddress gateway(192, 168, 1, 1);  
IPAddress subnet(255, 255, 255, 0);  
IPAddress primaryDNS(192, 168, 1, 1);  
IPAddress secondaryDNS(0, 0, 0, 0);
*/
IPAddress staticIP(192, 168, 2, 30);  
IPAddress gateway(192, 168, 2, 1);  
IPAddress subnet(255, 255, 255, 0);  
IPAddress primaryDNS(8, 8, 8, 8);  
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() 
{  
    Serial.begin(115200);  
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED) {  
        delay(1000);  
        Serial.print(".");  
    }  
    if (!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {  
        Serial.println("Failed to configure Static IP");  
    } else {  
        Serial.println("Static IP configured!");  
    }  
    Serial.print("ESP32 IP Address: ");  
    Serial.println(WiFi.localIP());  
}  
void loop() {  
    // Nothing to do here  
}  