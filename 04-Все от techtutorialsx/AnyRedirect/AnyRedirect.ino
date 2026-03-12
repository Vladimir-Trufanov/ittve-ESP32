// AnyRedirect.ino
// [HTTP-сервер ESP32 Arduino: внешние и внутренние перенаправления](https://techtutorialsx.wordpress.com/2018/01/14/esp32-arduino-http-server-external-and-internal-redirects/)

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Указываем учетные данные Wi-Fi
// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

AsyncWebServer server(80);

void setup()
{
  delay(5000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Подключение к WiFi ...");
  }
  Serial.println(WiFi.localIP());
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", "Hello from ESP32 server route");
  });

  server.on("/redirect/internal", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->redirect("/hello");
  });

  server.on("/redirect/external", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    // https://kwinflat.ru/
    request->redirect("https://kwinflat.ru/");
    //request->redirect("https://atomic-temporary-98295512.wpcomstaging.com/");
  });

  server.begin();
}

void loop(){}

