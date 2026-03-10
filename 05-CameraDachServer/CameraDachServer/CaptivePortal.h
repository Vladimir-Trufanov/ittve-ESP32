/*
В этом примере показан универсальный портал Captive для точки доступа ESP32.
Он позволяет современным устройствам и операционным системам определять, 
что подключение к Wi-Fi ограничено, и предлагать пользователю доступ к веб-странице с баннером. 

Нет необходимости искать и открывать IP-адрес/URL-адрес устройства, например http://192.168.4.1/

Это работает на Android, Ubuntu, FireFox, Windows и, возможно, на других платформах...
*/

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

DNSServer dnsServer;
WebServer server(80);

static const char responsePortal[] = R"===(
<!DOCTYPE html><html><head><title>ESP32 CaptivePortal</title></head><body>
<h1>Hello World!</h1><p>This is a captive <br> http://192.168.0.106 - для подключения к потоку <br> portal example page. All unknown http requests will
be redirected here.</p></body></html>
)===";

// index page handler
void handleRoot() 
{
  server.send(200, "text/plain", "Hello from esp32!");
}

// this will redirect unknown http req's to our captive portal page
// based on this redirect various systems could detect that WiFi AP has a captive portal page

// Это перенаправит неизвестные HTTP-запросы на нашу страницу с captive portal
// благодаря этому перенаправлению различные системы смогут определить,
// что у точки доступа Wi-Fi есть страница с captive portal
void handleNotFound() 
{
  server.sendHeader("Location", "/portal");
  server.send(302, "text/plain", "redirect to captive portal");
}

void PortalSetup() 
{
  //Serial.begin(115200);
  WiFi.AP.begin();
  WiFi.AP.create("CameraDachServer");
  WiFi.AP.enableDhcpCaptivePortal();

  // by default DNSServer is started serving any "*" domain name. It will reply
  // AccessPoint's IP to all DNS request (this is required for Captive Portal detection)
  // По умолчанию DNS-сервер настроен на обслуживание любого доменного имени со знаком «*».
  // Он будет отвечать IP-адресом точек доступа на все DNS-запросы 
  // (это необходимо для обнаружения Captive Portal).
  if (dnsServer.start())
  {
    Serial.println("Started DNS server in captive portal-mode");
  } 
  else 
  {
    Serial.println("Err: Can't start DNS server!");
  }

  // обслуживать простую корневую страницу
  server.on("/", handleRoot);

  // обслуживать страницу портала
  server.on("/portal", []() 
  {
    server.send(200, "text/html", responsePortal);
  });

  // все неизвестные страницы перенаправляются на защищенный портал
  server.onNotFound(handleNotFound);
  server.begin();
}

void PortalLoop() 
{
  server.handleClient();
  delay(5);  // give CPU some idle time
}
