
 // [1. Коды состояния ответа HTTP]
 // (https://developer.mozilla.org/ru/docs/Web/HTTP/Reference/Status)

 // [2. ESP32 WiFiMulti подключается к самой мощной сети Wi-Fi]
 // (https://microcontrollerslab.com/esp32-wifimulti-connect-to-the-strongest-wifi-network/)


// Определяем SSID собственной сети контроллера
// "Контроллер №1" - камера для съёмок на природе   ["DachaSad"]
// "ESP_75C391"    - ESP32-CAM, контроллер №3       ["NaDorogu"]
// "ESP_A7E119"    - ESP32-CAM, контроллер №6 
#define soft_ap_ssid "DachaSad" 


#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

WiFiMulti wifiMulti;

// callback used to check Internet connectivity
bool testConnection() {
  HTTPClient http;
  http.begin("http://www.espressif.com");
  int httpCode = http.GET();
  // we expect to get a 301 because it will ask to use HTTPS instead of HTTP
  if (httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
    return true;
  }
  return false;
}


// WARNING: This function is called from a separate FreeRTOS task (thread)!
void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
    case ARDUINO_EVENT_WIFI_READY:               Serial.println("WiFi interface ready"); break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:           Serial.println("Completed scan for access points"); break;
    case ARDUINO_EVENT_WIFI_STA_START:           Serial.println("WiFi client started"); break;
    case ARDUINO_EVENT_WIFI_STA_STOP:            Serial.println("WiFi clients stopped"); break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:       
      Serial.println("Connected to access point"); 
      Serial.println("Всем привет!"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:   
      Serial.println("WiFi отключился!"); 
      Serial.println("Disconnected from WiFi access point"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE: Serial.println("Authentication mode of access point has changed"); break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Obtained IP address: ");
      Serial.println(WiFi.localIP());
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:        Serial.println("Lost IP address and IP address is reset to 0"); break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:          Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_FAILED:           Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:          Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_PIN:              Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode"); break;
    case ARDUINO_EVENT_WIFI_AP_START:           Serial.println("WiFi access point started"); break;
    case ARDUINO_EVENT_WIFI_AP_STOP:            Serial.println("WiFi access point  stopped"); break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    Serial.println("Client connected"); break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("Client disconnected"); break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:   Serial.println("Assigned IP address to client"); break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:  Serial.println("Received probe request"); break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:         Serial.println("AP IPv6 is preferred"); break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:        Serial.println("STA IPv6 is preferred"); break;
    case ARDUINO_EVENT_ETH_GOT_IP6:             Serial.println("Ethernet IPv6 is preferred"); break;
    case ARDUINO_EVENT_ETH_START:               Serial.println("Ethernet started"); break;
    case ARDUINO_EVENT_ETH_STOP:                Serial.println("Ethernet stopped"); break;
    case ARDUINO_EVENT_ETH_CONNECTED:           Serial.println("Ethernet connected"); break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:        Serial.println("Ethernet disconnected"); break;
    case ARDUINO_EVENT_ETH_GOT_IP:              Serial.println("Obtained IP address"); break;
    default:                                    break;
  }
}

// WARNING: This function is called from a separate FreeRTOS task (thread)!
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected");
  Serial.println("IP address станции: ");
  //Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}
// WARNING: This function is called from a separate FreeRTOS task (thread)!
void WiFiDiscon(WiFiEvent_t event, WiFiEventInfo_t info) 
{
  Serial.println("Отключение");
  // Serial.println("IP address: ");
  ///Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}


/*
void Wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Successfully connected to Access Point");
}

void Get_IPAddress(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("WIFI is connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void Wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WIFI access point");
  Serial.print("WiFi lost connection. Reason: ");
  //Serial.println(info.disconnected.reason);
  Serial.println("Reconnecting...");
  //WiFi.begin(ssid, password);
}
*/


void setup() 
{
  Serial.begin(115200);
  delay(10);


  // delete old config
  //WiFi.disconnect(true);

  delay(1000);

  // Examples of different ways to register wifi events;
  // these handlers will be called from another thread.
  //WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(WiFiGotIP,  WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiDiscon, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  /*
  WiFiEventId_t eventID = WiFi.onEvent
  (
    [](WiFiEvent_t event, WiFiEventInfo_t info) 
    {
      Serial.print("WiFi lost connection. Reason: ");
      Serial.println(info.wifi_sta_disconnected.reason);
    },
    WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED
  );
  */

  // Remove WiFi event
  //Serial.print("WiFi Event ID: ");
  //Serial.println(eventID);




  WiFi.mode(WIFI_MODE_APSTA);
  
  // Создаём собственную сеть
  WiFi.softAP(soft_ap_ssid,soft_ap_ssid);
  
  wifiMulti.addAP("TP-Link_B394",  "18009217");
  wifiMulti.addAP("tve-DESKTOP",   "Ue18-647");
  wifiMulti.addAP("OPPO A9 2020",  "b277a4ee84e8");
  wifiMulti.addAP("tve-MONOBLOCK", "Ue18-647");
  wifiMulti.addAP("linksystve",    "X93K6KQ6WF");
  wifiMulti.addAP("GoshaIMila",    "t1s2wde4bE");

  // These options can help when you need ANY kind of wifi connection to get a config file, report errors, etc.
  wifiMulti.setStrictMode(false);  // Default is true.  Library will disconnect and forget currently connected AP if it's not in the AP list.
  wifiMulti.setAllowOpenAP(true);  // Default is false.  True adds open APs to the AP list.
  wifiMulti.setConnectionTestCallbackFunc(testConnection);  // Attempts to connect to a remote webserver in case of captive portals.

  //WiFi.onEvent(Wifi_connected,SYSTEM_EVENT_STA_CONNECTED);
  //WiFi.onEvent(Wifi_connected, WIFI_EVENT_STA_CONNECTED);
  //WiFi.onEvent(Get_IPAddress, SYSTEM_EVENT_STA_GOT_IP);
  //WiFi.onEvent(Get_IPAddress, IP_EVENT_STA_GOT_IP);
  //WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  //WiFi.onEvent(Wifi_disconnected, WIFI_EVENT_STA_DISCONNECTED); 
  
  
  
  
  // Отключаем режим сна (modem sleep). 
  WiFi.setSleep(false);
  Serial.print("Подключение к WiFi ");
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
  Serial.print("IP собственной сети: ");  Serial.print(WiFi.softAPIP()); Serial.print("  "); Serial.println(soft_ap_ssid);
  Serial.print("IP рабочей станции:  ");  Serial.print(WiFi.localIP());  Serial.print("  "); Serial.println(WiFi.SSID());
  Serial.print("RSSI = "); Serial.println(WiFi.RSSI());
}

void loop() 
{
  static bool isConnected = false;
  uint8_t WiFiStatus = wifiMulti.run();

  if (WiFiStatus == WL_CONNECTED) {
    if (!isConnected) {
      Serial.println("");
      Serial.println("WiFi connected2");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    isConnected = true;
  } else {
    Serial.println("WiFi not connected!");
    isConnected = false;
    delay(5000);
  }
}
