/** Arduino, ESP32, C/C++ **************************** CameraDachServer.ino ***
 * 
 * v4.0.6, 11.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
 * Preferences:       https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
 * Espressif Systems: Esp32 от Espressif Systems версии 3.3.5 [3.0.7, 2.0.2] 
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

#include "esp_camera.h"
#include <WiFi.h>

// Выбираем модель камеры
#include "board_config.h"
// Указываем учетные данные Wi-Fi
// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";
// "DachaSad" - камера для съёмок на природе
const char* soft_ap_ssid     = "DachaSad";  
const char* soft_ap_password = "DachaSad";
// "Proba3" = "ESP_75C391" - камера на 3-ем контроллере
//const char* soft_ap_ssid     = "Proba3";  
//const char* soft_ap_password = "Proba3";

//#define CORE_DEBUG_LEVEL 5

// Настраиваем логирование скетча
// #include "project_config.h"                        // не подключаем конфигурационный файл
#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_VERBOSE  // выводим сообщения всех уровней
#define CONFIG_RLOG_SHOW_TIMESTAMP 0                  // не выводим отметок времени
#define CONFIG_RLOG_SHOW_FILEINFO 0                   // не выводим отметку о месте сообщения в скетче
#include "rLog.h"                                      
static const char* rl = "cds";                        // указали тег сообщений


void startCameraServer();
void setupLedFlash();

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  delay(5000);
  
  // Показываем контрольные определения
  #include "ctrl_define.h"
  //CtrlDefine();
  
  // Конфигурируем камеру 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  // 2026-03-11 в коде приложения оставлен весь путь работы с параметром xclk, как пример.
  // Пример, можно отследить по меткам ***xclk***:
  // ***xclk***=1, устанавливаем рекомендованную частоту входного тактового сигнала 
  // для камеры OV2640 = 20 МГц (это единственная строка, связанная с xclk, которая не закоментирована). 
  // ***xclk***=2, регистрация URI-обработчика HTTP-запроса по изменению частоты
  // входного тактового сигнала.
  // ***xclk***=3, обработчика HTTP-запроса по изменению частоты
  // входного тактового сигнала
  // ***xclk***=4, готовим значение частоты входного тактового сигнала
  // для передачи на страницу управления камерой
  // ***xclk***=5, отрабатываем клик на изменение частоты входного тактового сигнала
  // в Мгц на странице управления камерой
  // ***xclk***=6, запускаем страницу на изменение частоты входного тактового сигнала
  // ***xclk***=7, "set-xclk-group" в CSS делаем невидимым
  // ***xclk***=8, "set-xclk-group" в CSS делаем невидимым 
  // ***xclk***=9, подключаем обработчик по изменению частоты тактового сигнала
  config.xclk_freq_hz = 20000000;
  // 2026-03-11 в коде приложения на странице управления камерой 
  // убираем некоторые форматы изображения:
  // <!--option value="7">320x320</option--> <!-- Unsupported on ov2640 -->
  // <option value="5">240x240</option>
  // <option value="4">HQVGA(240x176)</option>
  // <option value="3">QCIF(176x144)</option>
  // <option value="2">128x128</option>
  // <option value="1">QQVGA(160x120)</option>
  // <option value="0">96x96</option>
  config.pixel_format = PIXFORMAT_JPEG;     
  // По присутствию PSRAM, для более высокого качества JPEG определяемся с 
  // разрешением и выделением буфера кадров.
  config.frame_size = FRAMESIZE_UXGA;
  config.grab_mode  = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  if (psramFound()) 
  {
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } 
  else 
  {
    // Ограничиваем размер кадра, если PSRAM недоступна
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }
  // Инициализируем камеру
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Ошибка инициализации камеры 0x%x", err);
    return;
  }
  // Выполняем начальную перенастройку кадров
  sensor_t *s = esp_camera_sensor_get();
  // По размеру кадра устанавливаем частоту кадров
  if (config.pixel_format == PIXFORMAT_JPEG) 
  {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
  // Назначаем работу контроллера, как станции WiFi и с собственной сетью
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(soft_ap_ssid, soft_ap_password);
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
  Serial.print("IP собственной сети: ");  Serial.print(WiFi.softAPIP()); Serial.print("  "); Serial.println(soft_ap_ssid);
  Serial.print("IP рабочей станции:  ");  Serial.print(WiFi.localIP());  Serial.print("  "); Serial.println(ssid);

  // Если статический адрес для TP-Link_B394
  /*
  Serial.print("Current ESP32 IP: ");      Serial.println(WiFi.localIP());
  Serial.print("Gateway1 (router) IP: ");  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: " );          Serial.println(WiFi.subnetMask());
  Serial.print("Primary DNS: ");           Serial.println(WiFi.dnsIP(0));
  Serial.print("Secondary DNS: ");         Serial.println(WiFi.dnsIP(1));
  // Static IP configuration
  IPAddress staticIP(192, 168, 0, 200); // ESP32 static IP
  IPAddress gateway(192, 168, 0, 1);    // IP Address of your network gateway (router)
  IPAddress subnet(255, 255, 255, 0);   // Subnet mask
  IPAddress primaryDNS(192, 168, 0, 1); // Primary DNS (optional)
  IPAddress secondaryDNS(0, 0, 0, 0);   // Secondary DNS (optional)
  // Configuring static IP
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) 
  {
    Serial.println("Failed to configure Static IP");
  } 
  else 
  {
    Serial.println("Static IP configured!");
  }
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address to Serial Monitor
  */
  
  startCameraServer();

  Serial.print("Камера готова! \n'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' для подключения к потоку.");
 }

void loop() 
{
  // В фоновом цикле ничего не делается. 
  // Трансляция потока выполняется веб-сервером в другой задаче
  delay(100);  // give CPU some idle time
}

// *************************************************** CameraDachServer.ino ***
