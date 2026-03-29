/** Arduino, ESP32, C/C++ **************************** CameraDachServer.ino ***
 * 
 * v4.0.9, 29.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 26.02.2026
 * 
 * Preferences:       https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
 * Espressif Systems: Esp32 от Espressif Systems версии 3.3.5 [3.0.7, 2.0.2] 
 * Payment:           "Al Thinker ESP32-CAM"
 * CPU Frequency:     "240MHz (WiFi/BT)"
 * Flash Frequency:   "80MHz"
 * Flash Mode:        "QIO"
**/

// Обеспечиваем хранение в NVS (счетчик перезапусков, дата и время)
#include <Arduino.h>
#include "ArduinoNvs.h"
// Подключаем библиотеки для подключения камеры к WiFi
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
// Подключаем библиотеку для связи с сервером SNTP, которая является 
// библиотекой ядра ESP32 по умолчанию и не требуют установки
#include "esp_sntp.h"
// Выбираем модель камеры
#include "esp_camera.h"
#include "board_config.h"
#include "ctrl_define.h"
#include "jpr.h"

void IniSayWiFi();
void startCameraServer();
void setupLedFlash();

static unsigned long currentMillis;  // текущее время в миллисекундах 
static bool isWiFi=false;  

// ****************************************************************************
// * Настроить интервал синхронизации, имя сервера, режим работы и часовой пояс
// ****************************************************************************
void notify(struct timeval* t) 
{
  delay(700);
  Serial.println("Время синхронизировано!");
}
void initSNTP() 
{ 
   // Определяем, как часто синхронизировать внутренние часы ESP32 с сервером 
   // SNTP. Интервал указывается в микросекундах. Интервал в 60*60*1000UL 
   // микросекунд означает синхронизацию каждый час. Разумные интервалы запросов 
   // обычно составляют от одного-двух раз в день до 5 раз в час. 
   sntp_set_sync_interval(1 * 59 * 60 * 1000UL);  // 59 минут 
   // Указываем функцию уведомления (callback), которая вызывается при каждой с
   // инхронизации. В данном коде для этой цели определяем функцию notify(), 
   // которая просто выводит «synchronized». 
   sntp_set_time_sync_notification_cb(notify);
   // Устанавливаем режим работы: ESP_SNTP_OPMODE_POLL — просто опрашивать
   // сервер SNTP (есть также ESP_SNTP_OPMODE_LISTENONLY)
   esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
   // Указываем имя/адрес сервера. При желании можно указать несколько серверов.
   // Например:
   // "ntp.msk-ix.ru"
   //           esp_sntp_setservername(0, «pool.ntp.org»);
   //           esp_sntp_setservername(1, «de.pool.ntp.org»);
   //           esp_sntp_setservername(2, «time.nist.gov»);
   esp_sntp_setservername(0, "ntp.msk-ix.ru");
   // Запускаем службу SNTP с указанными выше параметрами
   esp_sntp_init();
   // Устанавливаем часовой пояс, поскольку сервер SNTP 
   // возвращает время в формате UTC
   setTimezone();
}
// ****************************************************************************
// *                           Настроить часовой пояс                         *
// ****************************************************************************
void setTimezone() 
{ 
   // Здесь устанавливается стандартное время для региона - Европа/Москва
   // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
   setenv("TZ", "MSK-3", 1);
   tzset();
}
// ****************************************************************************
// *                            Ожидать синхронизацию                         *
// ****************************************************************************
void wait4SNTP() 
{
  Serial.println("Ожидание синхронизации ");
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
  {
    delay(450);
    Serial.print(".");
  }
  Serial.println(" ");
}
// ****************************************************************************
// *   Извлечь информацию о текущем времени и вывести ее в отформатированном  *
// *   виде с помощью struct tm структуры данных:                             *
// *                               https://cplusplus.com/reference/ctime/tm/  *
// ****************************************************************************
/*
 "%A, %B %d %Y %H:%M:%S" - это спецификаторы формата,  которые определяют,  как
 в struct tm timeinfo; будет отформатирован текст, а члены tm struct следующие:
  
  Тип элемента Значение                        Диапазон
  -----------------------------------------------------
  tm_sec  int  секунды после минуты            0-61*
  tm_min  int  минуты после часа               0-59
  tm_hour int  часы с полуночи                 0-23
  tm_mday int  день месяца                     1-31
  tm_mon  int  месяцы с января                 0-11
  tm_year int  годы с 1900
  tm_wday  —   количество дней с воскресенья   0-6
  tm_yday  —   количество дней с 1 января      0-365
  tm_isdst —   флаг перехода на летнее время 
  
  function strftime() - format time as string:
  https://cplusplus.com/reference/ctime/strftime/
*/
void printTime() 
{
   struct tm timeinfo;
   getLocalTime(&timeinfo);
   Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

uint32_t boot_count;  // счетчик перезапусков

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  delay(5000);

  // Инициируем NVS
  NVS.begin();
  // После каждой перезагрузки увеличиваем счётчик загрузок и выводим в терминал.
  if (NVS.getInt("boot_count")) 
  {
    Serial.println("есть число");
    boot_count = NVS.getInt("boot_count"); 
    boot_count++;
  }
  else 
  {
    Serial.println("нет числа!");
    boot_count = 1; 
  }
  NVS.setInt("boot_count", boot_count);
  printf("Счетчик перезапусков = %d\n", boot_count);
  /*
  const uint32_t ui32_set = 4294967295;
  NVS.setInt("unsigned-long", ui32_set);
  uint32_t uint32_max = NVS.getInt("unsigned-long"); 
  printf("МАХ целое без знака: в десятичном виде = %u, в шестнадцатеричном = %#x"  "\n", uint32_max, uint32_max);
  */
  /*
   // Записываем в NVS и читаем строку
   const String st_set = "Это простая незамысловатая строка для записи в NVS";
   res = NVS.setString("str", st_set);
   String str = NVS.getString("str");
   Serial.println(str);
   // Записываем и читаем двоичные данные переменной длины (blob)
   uint8_t blolb_set[8] = {1,2,3,99,100,0xEE,0xFE,0xEE};
   res = NVS.setBlob("blob", blolb_set, sizeof(blolb_set));
   size_t blobLength = NVS.getBlobSize("blob"); 
   uint8_t blob[blobLength];
   res = NVS.getBlob("blob", blob, sizeof(blob));
   if (res) 
   {
      for (uint8_t i = 0; i < blobLength; i++) 
      {
         Serial.printf("blob[%u] = %u; ", i, blob[i]);
      }
   }
   else
   {
      Serial.println("Не получилось извлечь BLOB из NVS");
   }
  */
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
  // ***xclk***=1, устанавливаем рекомендованную частоту входного тактового сигнала 
  // (есть возможность установить значение config.xclk_freq_hz = 10000000 для более низкой частоты 
  // обновления, но это может привести к стабильному (более медленному) выполнению. Для ESP32-S3-WROOM 
  // есть пример, в котором указано значение config.xclk_freq_hz = 24000000 — это 24 МГц XCLK (самая 
  // высокая стабильная частота). Частота XCLK влияет на работу ESP32-CAM, в частности на скорость 
  // передачи данных и качество изображения. Теоретически, чем выше частота XCLK, тем быстрее будет 
  // передача данных. Однако на практике высокая частота XCLK требует большей скорости обработки чипа. 
  // Кроме того, слишком высокая частота XCLK может привести к тому, что параллельные данные будут несинхронными, 
  // что вызовет дрожание изображения или даже мерцание экрана. 
  // Для достижения оптимальной частоты кадров и качества изображения необходимо сбалансировать 
  // частоту XCLK в соответствии с конкретными сценариями применения. Например, для достижения 60 кадров 
  // в секунду в формате CIF необходима работа на частоте XCLK 24 МГц, в то время как на частоте 20 МГц 
  // можно достичь только 50 кадров в секунду. 
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
  // Инициируем работу контроллера, как станции WiFi и с собственной сетью
  // print_mem(" WIFI_MODE_APSTA"); 
  WiFi.mode(WIFI_MODE_APSTA);
  char* soft_apssid = soft_ap_ssid;      // не более 10 символов, латиница
  WiFi.softAP(soft_apssid, soft_apssid);
  // Включаем обработку события отсоединения с WiFi через безымянную лямбда-функцию
  // (так как это выполняется в отдельном потоке, то просто устанавливаем флаг,
  // чтобы его обнарудить в цикле loop и переподключить WiFi) 
  WiFiEventId_t eventID = WiFi.onEvent
  (
    [](WiFiEvent_t event, WiFiEventInfo_t info) 
    {
      //Serial.print("Wi-Fi потерял связь. Причина: ");
      //Serial.println(info.wifi_sta_disconnected.reason);
      isWiFi=false;  
    },
    WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED
  );
  // Привязываем возможные сети и подключаемся к WiFi
  wifiMulti.addAP("TP-Link_B394",  "18009217");
  wifiMulti.addAP("tve-DESKTOP",   "Ue18-647");
  wifiMulti.addAP("OPPO A9 2020",  "b277a4ee84e8");
  wifiMulti.addAP("tve-MONOBLOCK", "Ue18-647");
  wifiMulti.addAP("linksystve",    "X93K6KQ6WF");
  wifiMulti.addAP("GoshaIMila",    "t1s2wde4bE");
  Serial.println("Подключается WiFi ... ");
  delay(100);
  if (wifiMulti.run() == WL_CONNECTED) 
  {
    Serial.println("WiFi подсоединен в Setup");
    isWiFi=true; 
  }

  initSNTP();
  wait4SNTP();

  IniSayWiFi();
  // print_mem("после IniSayWiFi"); 

  // Запускаем в работу камеру                    
  startCameraServer();
  Serial.print("Камера готова! \n'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' для подключения к потоку.");
}

void loop() 
{
  // Выводим контрольное сообщение после каждых 30 секунд
  if ((millis()-currentMillis) > 30000) 
  { 
    Serial.print("[30 сек] "); printTime();
    currentMillis = millis();
  }  
  //
  if (!isWiFi)
  {
    Serial.println("Подключается WiFi после потери сети");
    while (wifiMulti.run() != WL_CONNECTED) 
    { 
      Serial.print("."); delay(500);
    }
    Serial.println(" ");
    Serial.println("WiFi подсоединен!");
    IniSayWiFi();
    isWiFi=true; 
  }
  // Трансляция потока выполняется веб-сервером в другой задаче
  delay(10);  
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
