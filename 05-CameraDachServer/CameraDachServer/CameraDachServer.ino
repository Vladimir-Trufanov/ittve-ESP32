/** Arduino, ESP32, C/C++ **************************** CameraDachServer.ino ***
 * 
 * v4.0.7, 13.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
 * Preferences:       https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
 * Espressif Systems: Esp32 от Espressif Systems версии 3.3.5 [3.0.7, 2.0.2] 
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

#include <WiFi.h>
#include "esp_camera.h"

// Выбираем модель камеры
#include "board_config.h"
#include "ctrl_define.h"

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

void InitWiFi(const char* ssid, const char* password, const char* soft_ap_ssid, const char* soft_ap_password);
void startCameraServer();
void setupLedFlash();

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  delay(5000);
  
  // Показываем контрольные определения
  // CtrlDefine();
  log_i("Контрольная проверка %s", "логирования");

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
    //Serial.printf("Ошибка инициализации камеры 0x%x", err);
    log_i("Ошибка инициализации камеры 0x%x", err);
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
  InitWiFi(ssid, password, soft_ap_ssid, soft_ap_password);
  Serial.print("IP собственной сети: ");  Serial.print(WiFi.softAPIP()); Serial.print("  "); Serial.println(soft_ap_ssid);
  Serial.print("IP рабочей станции:  ");  Serial.print(WiFi.localIP());  Serial.print("  "); Serial.println(ssid);
  
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

// 2026-03-14 в коде приложения проходим путь сохранения фотографий, ***save***:
// ***save***=1, кнопка сохранения фотографии
// ***save***=2, связали кнопку с переменной
// ***save***=3, отрабатываем кнопку сохранения фотографии

// *************************************************** CameraDachServer.ino ***
