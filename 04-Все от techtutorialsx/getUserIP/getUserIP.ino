// getUserIP.ino
// [Веб-сервер ESP32 Arduino: получение IP-адреса клиента](https://techtutorialsx.wordpress.com/2018/02/17/esp32-arduino-web-server-getting-client-ip/)

/*
  Тестирование кода.
  Чтобы протестировать код, нужно выполнить обычную процедуру. Сначала 
мы компилируем и загружаем код в ESP32 с помощью Arduino IDE.
  Когда процедура завершится, откройте последовательный монитор 
и скопируйте выведенный на экран IP-адрес. Затем откройте любой 
веб-браузер и введите в адресную строку следующее, заменив #yourDeviceIp# 
на только что скопированный IP-адрес.

http://#yourDeviceIp#/printIp
http://192.168.0.106/printIp

  В веб-браузере вы должны увидеть ответ «ok», который мы указали в коде.
Если вы вернетесь к последовательному монитору, то увидите сообщение
 с локальным IP-адресом компьютера, с которого вы пытались связаться с ESP32.
*/

#include <WiFi.h>
//#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Указываем учетные данные Wi-Fi
// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";
 
AsyncWebServer server(80);
 
void setup()
{
  Serial.begin(115200);
  delay(5000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.print("Server IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("\n\n");
 
  server.on("/printIp", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    request->send(200, "text/plain", "ok");
 
    Serial.print("Received request from client with IP: ");
    Serial.println(request->client()->remoteIP());
  });
 
  server.begin();
}
 
void loop(){}