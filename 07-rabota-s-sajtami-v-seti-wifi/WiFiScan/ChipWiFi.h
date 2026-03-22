/** Arduino, ESP32, C/C++ ************************************** ChipWiFi.h ***
 * 
 *                          Обеспечить сканирование сетей WiFi, выбор и ведение 
 *                           локальной сети WiFi и собственной сети контроллера 
 *                                                     
 * v1.0.3, 22.03.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 18.03.2026
 *
 * 09:27:03.783 -> ESP_IDF_VERSION              = 328961
 * 09:27:03.783 -> ESP_IDF_VERSION_VAL(5, 4, 2) = 328706
**/

#pragma once     

#include "WiFi.h"

// Определяем SSID собственной сети контроллера
#ifndef soft_ap_ssid
  #define soft_ap_ssid "Esp-DachaSad" 
#endif
// Формируем данные массива учетных записей возможных локальных сетей
int enarr = 6;         // число элементов в массиве
const char* esarr[] =  // массив учетных данных
{
  "tve-DESKTOP",   "Ue14-328",
  "OPPO A9 2020",  "b277a4ee84e8",
  "tve-MONOBLOCK", "Ue17-647",
};

// Резервируем параметры для выбираемой сети
char essid[34] = {0};       // идентификатор беспроводной сети
char epassword[34] = {0};   // пароль
int eRSSI = -999;           // уровень принимаемого сигнала (дБм - децибел на милливатт) 

// Проверить и сообщить статус соединения с WiFi                
wl_status_t CheckWiFiStatus();
// Проверить и сообщить, какой включен режим работы WiFi  
WiFiMode_t CheckWiFiMode(); 
// Просканировать, показать сети WiFi в диапазоне и выбрать с большим RSSI 
bool ViewWiFi(const char* sarr[]=esarr, int narr=enarr); 
// Просканировать сети WiFi в диапазоне по списку и выбрать подходящую для подключения 
bool ScanWiFi(const char* sarr[], int narr); 
// Проверить соответствие заданному ssid одной из сетей WiFi и выбрать сеть для подключения 
bool findWiFi(const char* ssid, int iRSSI, const char* sarr[], int narr);
// Инициировать работу контроллера, как станции WiFi и с собственной сетью
void InitWiFi(const char* ssid, const char* password);

// ****************************************************************************
// *     Проверить соответствие заданному ssid одной из сетей WiFi, которые   *
// *      может использовать контроллер и выбрать эту сеть для подключения    *
// *        [essid,epassword], если уровень сигнала превышает ранее           *
// *                    зафиксированный уровень другой сети                   *
// ****************************************************************************
bool findWiFi(const char* ssid, int iRSSI, const char* sarr[], int narr) 
{
  bool result=false;
  for (int i = 0; i < narr; i=i+2) 
  {
    //Serial.print("Смотри: "); Serial.print(iRSSI); Serial.print(" "); Serial.print(sarr[i]); Serial.print(" => ");  Serial.println(sarr[i+1]);
    int res = strcmp(sarr[i], ssid);
    if (res == 0) 
    {
      //Serial.print("Совпал: "); Serial.print(iRSSI); Serial.print(" "); Serial.print(sarr[i]); Serial.print(" => ");  Serial.println(sarr[i+1]);
      if (((strlen(sarr[i]))<16) || ((strlen(sarr[i]))<16))
      {
        //Serial.print("Годен:  "); Serial.print(iRSSI); Serial.print(" "); Serial.print(sarr[i]); Serial.print(" => ");  Serial.println(sarr[i+1]);
        //Serial.print("Было:   "); Serial.print(eRSSI); Serial.print(" "); Serial.print(essid);   Serial.print(" => ");  Serial.println(epassword);
        if (iRSSI>eRSSI)
        {
          // essid[16] = {0};
          // Serial.print(essid); Serial.print(" 0=> ");  Serial.println(epassword);
          strncpy(essid, sarr[i], sizeof(essid) - 1); 
          // epassword[16] = {0};
          strncpy(epassword, sarr[i+1], sizeof(epassword) - 1); 
          eRSSI=iRSSI;
          //Serial.print("Стало:  "); Serial.print(eRSSI); Serial.print(" "); Serial.print(essid);   Serial.print(" => ");  Serial.println(epassword);
          result=true;
          break;
        }
      }
    }
  }
  return result; 
}
// ****************************************************************************
// *              Просканировать сети WiFi в диапазоне по списку и            *
// *                     выбрать подходящую для подключения                   *
// ****************************************************************************
void SerialDefis()
{
  Serial.println("--------------------------------------------------------------");
}
bool ScanWiFi(const char* sarr[], int narr) 
{
  // Отмечаем, что подходящая сеть ещё не найдена
  bool isnetwifi=false;
  //Serial.println("Начато сканирование сетей WiFi");
  int n = WiFi.scanNetworks(); // получили количество найденных сетей
  //Serial.println("Сканирование завершено");
  if (n == 0) 
  {
    Serial.println("Сетей для сканирования не найдено");
  } 
  else 
  {
    //Serial.print(n); Serial.println(": столько сетей найдено");
    Serial.println("№  | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i) 
    {
      // Выводим данные текущей сети
      Serial.printf("%2d", i + 1);
      Serial.print(" | ");
      Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
      Serial.print(" | ");
      Serial.printf("%4ld", WiFi.RSSI(i));
      Serial.print(" | ");
      Serial.printf("%2ld", WiFi.channel(i));
      Serial.print(" | ");
      switch (WiFi.encryptionType(i)) 
      {
        case WIFI_AUTH_OPEN:            Serial.print("open"); break;
        case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
        default:                        Serial.print("unknown");
      }
      Serial.println();
      // Отмечаем, если выбрана подходящая сеть
      bool isWiFi=findWiFi(WiFi.SSID(i).c_str(),WiFi.RSSI(i),sarr,narr);
      if (isWiFi) 
      {
        //Serial.print("Можно подключиться: ");
        //Serial.print(essid); Serial.print(" => "); Serial.print(epassword); Serial.print(" = ");  Serial.println(eRSSI);
        isnetwifi=true;
      }
      delay(10);
    }
  }
  // Удаляем результат сканирования, чтобы освободить память
  WiFi.scanDelete();
  SerialDefis();
  return isnetwifi;
}
// ****************************************************************************
// *  Просканировать, показать сети WiFi в диапазоне и выбрать с большим RSSI *
// ****************************************************************************
bool preViewWiFi(const char* sarr[], int narr)
{
  bool iswifiview=true;
  // Для ранних версий ESP_IDF не выделяем отдельных диапазонов
  SerialDefis();
  Serial.println("Отсканированные сети Wi-Fi:");
  SerialDefis();
  // Для поздних версий ESP_IDF указываем автоматический выбор диапазона 
  #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 2)
    WiFi.setBandMode(WIFI_BAND_MODE_AUTO);
  #endif
  iswifiview=ScanWiFi(sarr,narr);
  /*
  // Для поздних версий ESP_IDF с поддержкой 5G вначале выбираем диапазон 2.4 Ghz 
  #if CONFIG_SOC_WIFI_SUPPORT_5G
    delay(1000);
    SerialDefis();
    Serial.println("Отсканированные сети в диапазоне WiFi 2,4 ГГц:");
    SerialDefis();
    WiFi.setBandMode(WIFI_BAND_MODE_2G_ONLY);
    iswifiview=ScanWiFi(sarr,narr);
    delay(1000);
    // Затем выбираем диапазон 5 Ghz 
    SerialDefis();
    Serial.println("Отсканированные сети в диапазоне WiFi 5 ГГц");
    SerialDefis();
    WiFi.setBandMode(WIFI_BAND_MODE_5G_ONLY);
    iswifiview=ScanWiFi(sarr,narr);
  #endif
  */
  //if (iswifiview) Serial.println("Точно сеть найдена!");
  //else Serial.println("Точно сеть НЕ найдена!");
  return iswifiview;
}
bool ViewWiFi(const char* sarr[], int narr)
{
  bool iswifiview=true;
  int statusWiFi;
  // Сбрасываем начальный уровень принимаемого сигнала (дБм - децибел на милливатт) 
  eRSSI = -999;          
  // Трассируем итоговый массив учетных записей возможных локальных сетей
  // for (int i = 0; i < narr; ++i) Serial.println(sarr[i]);
  /*
  При вызове функции WiFi.mode(WIFI_STA) в Arduino с модулем ESP32 могут возникать ошибки из-за проблем 
  с подключением к точке доступа Wi-Fi или сбоев в работе драйвера WiFi. Для обработки ошибок рекомендуется 
  использовать обработчики событий и учитывать причины их возникновения. 
    Некоторые возможные причины ошибок:
  - неудачная попытка подключения к точке доступа. Например, ESP32 не может найти имя сети (SSID) или подключиться к ней;
  - разрыв соединения Wi-Fi, из-за чего возникает событие WIFI_EVENT_STA_DISCONNECTED;
  - принудительный вызов функции esp_wifi_disconnect — в этом случае приложение не должно вызывать 
  esp_wifi_connect() для повторного переподключения;
  - временные проблемы с подключением, например, из-за слабого уровня сигнала или высокого уровня помех. 
    Некоторые методы обработки ошибок:
  - мониторинг статуса подключения с помощью функции WiFi.status. Она возвращает целое число, 
  соответствующее текущему статусу соединения. Например, WL_NO_SSID_AVAIL означает, что ESP32 
  не может найти имя сети, WL_CONNECT_FAILED — что ESP32 не может подключиться к назначенной сети;
  - обработка события WIFI_EVENT_STA_DISCONNECTED. В обработчике можно вызвать esp_wifi_connect() 
  для попытки повторного подключения. Однако если событие возникает из-за принудительного вызова 
  esp_wifi_disconnect(), приложение не должно вызывать esp_wifi_connect();
  - ограничение количества попыток переподключения. Если количество неудачных попыток превысило 
  лимит (например, CONFIG_WIFI_RESTART_ATTEMPTS по умолчанию), нужно выйти из бесконечного цикла 
  переподключений и попробовать что-то ещё;
  - использование группы событий для хранения текущего состояния подключения. При получении события 
  WIFI_EVENT в обработчике можно устанавливать заранее определённый бит (флаг) в группе. Тогда 
  любая задача сможет в любой момент времени проверить, есть ли подключение к точке доступа 
  или его нет;
  - перезапуск ESP32 — если ESP32 временно не подключается к Wi-Fi по неизвестным причинам, 
  можно добавить тайм-аут и использовать функцию ESP.restart для перезапуска ESP32 из кода.
    Важно: бесконечный цикл подключения к одной и той же точке доступа не всегда идеален — например, 
  если внезапно роутер вышел из строя. Поэтому в реальных сценариях стоит предпринять какие-то 
  специальные меры в случае неудачи, например, попробовать подключиться к резервной сети. 
  [Wi-Fi API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html)
  */
  // Ждем немного перед сканированием
  delay(100);
  // Проверяем режим работы WiFi
  //CheckWiFiMode();
  //int modeWiFi=WiFi.getMode();
  Serial.print("WiFi.getMode() = "); Serial.println(WiFi.getMode());
  // Проверяем статус
  statusWiFi=WiFi.status();
  Serial.print("WiFi.status()  = "); Serial.println(statusWiFi);
  // 1. Если неопределенный статус подключения к сети, то сразу переходим в режим сканирования
  if ((statusWiFi>6)&&(statusWiFi<255))
  {
    iswifiview=preViewWiFi(sarr,narr);
    return iswifiview;
  }
  // 2. Если нет подключения к сети, то
  // --- переводим Wi-Fi в режим станции и отключаемся от точки доступа - 
  // --- в таких обстоятельствах выполняем сканирование сети
  /*
  else if (statusWiFi==6)
  {
    iswifiview=WiFi.mode(WIFI_STA);
    if (!iswifiview) 
    {
      Serial.println("Не удалось установить режим работы станции (ошибка WiFi.mode(WIFI_STA)"); 
      return false;
    }
    / *
    iswifiview=WiFi.mode(WIFI_STA);
    if (!iswifiview) 
    {
      Serial.println("Не удалось установить режим работы станции (ошибка WiFi.mode(WIFI_STA)"); 
      return false;
    }
    * /
    / *
    iswifiview=WiFi.disconnect();
    if (!iswifiview) 
    {
      Serial.println("Не удалось отключиться от Wi-Fi перед сканированием"); 
      return false;
    }
    * /
    iswifiview=preViewWiFi(sarr,narr);
    return iswifiview;
  }
  */
  // 3. ------Отменяем сканирование в остальных случаях
  else
  {
    iswifiview=WiFi.mode(WIFI_OFF);
    if (!iswifiview) 
    {
      Serial.println("---------Не удалось установить режим работы станции (ошибка WiFi.mode(WIFI_STA)"); 
      return false;
    }
    // Ждем немного перед сканированием
    delay(100);
    // Проверяем режим работы WiFi
    //CheckWiFiMode();
    //int modeWiFi=WiFi.getMode();
    Serial.print("WiFi.getMode() = "); Serial.println(WiFi.getMode());
    // Проверяем статус
    //statusWiFi=WiFi.status();
    Serial.print("WiFi.status()  = "); Serial.println(WiFi.status());
    iswifiview=preViewWiFi(sarr,narr);
    return iswifiview;
  }
}
// ****************************************************************************
// *   Инициировать работу контроллера, как станции WiFi и с собственной сетью  *
// ****************************************************************************
void InitWiFi(const char* ssid, const char* password)
{
  // Переключаем контроллер в режим работы с собственной сетью и как станции
  WiFi.mode(WIFI_MODE_APSTA);
  // Создаём собственную сеть
  WiFi.softAP(soft_ap_ssid,soft_ap_ssid);
  // Подключаемся к WiFi
  WiFi.begin(ssid,password);
  // Отключаем режим сна (modem sleep). 
  WiFi.setSleep(false);
  Serial.print("Подключение к WiFi ");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
  Serial.print("IP собственной сети: ");  Serial.print(WiFi.softAPIP()); Serial.print("  "); Serial.println(soft_ap_ssid);
  Serial.print("IP рабочей станции:  ");  Serial.print(WiFi.localIP());  Serial.print("  "); Serial.println(ssid);
}
// ****************************************************************************
// *            Проверить и сообщить статус соединения с WiFi                 *
// * https://github.com/arduino/esp8266/blob/master/libraries/ESP8266WiFi/src/include/wl_definitions.h
// ****************************************************************************
wl_status_t CheckWiFiStatus() 
{
  wl_status_t wifiStatus = WiFi.status();
  switch (wifiStatus) 
  {
    // Cтандартный статус перед попыткой подключиться к WiFi-сети. Возвращается, 
    // когда функция WiFi.begin() вызвана и остаётся активной. Если количество 
    // попыток подключения будет исчерпано, этот статус меняется на WL_CONNECT_FAILED,
    // а если соединение будет успешно установлено, то на WL_CONNECTED
    case WL_IDLE_STATUS:      // 0 
      Serial.println("Попытка подключения к WiFi");
      break;
    // ESP32 не смог найти WiFi-сеть (например, сеть слишком далеко от ESP32 или SSID сети неправильный)
    case WL_NO_SSID_AVAIL:    // 1
      Serial.println("Не удалось найти сеть по SSID");
      break;
    // Завершено сканирование ближайших WiFi-сетей;
    case WL_SCAN_COMPLETED:   // 2
      Serial.println("Завершено сканирование ближайших WiFi-сетей");
      break;
    // Произошло успешное подключение к WiFi-сети (AP)
    case WL_CONNECTED:        // 3
      Serial.println("Соединение с WiFi-сетью установлено");
      break;
    // Подключение к WiFi-сети (AP) не удалось
    case WL_CONNECT_FAILED:   // 4
      Serial.println("Подключение к WiFi-сети не удалось");
      break;
    // Cоединение с WiFi-сетью потеряно
    case WL_CONNECTION_LOST:  // 5
      Serial.println("Cоединение с WiFi-сетью потеряно");
      break;
    // Контроллер отключён от WiFi-сети
    case WL_DISCONNECTED:     // 6
      Serial.println("Контроллер отключён от WiFi-сети");
      break;
    // Не подключён WiFi-модуль
    case WL_NO_SHIELD:        // 255
      Serial.println("Не подключён WiFi-модуль");
      break;
    default:
      Serial.println("Неопределенный статус подключения к WiFi");
      break;
  }
  return wifiStatus;
}
// ****************************************************************************
// *           Проверить и сообщить, какой включен режим работы WiFi          *
// ****************************************************************************
WiFiMode_t CheckWiFiMode() 
{
  WiFiMode_t wifiMode = WiFi.getMode();
  switch (wifiMode) 
  {
    case WIFI_OFF:           // 0 
      Serial.println("Wi-Fi выключен");
      break;
    case WIFI_STA:           // 1
      Serial.println("Включен режим станции - STA (подключение к AP)");
      break;
    // Завершено сканирование ближайших WiFi-сетей;
    case WIFI_AP:            // 2
      Serial.println("Включен режим собственной точки доступа - AP");
      break;
    // Произошло успешное подключение к WiFi-сети (AP)
    case WIFI_AP_STA:        // 3
      Serial.println("Включены режимы STA и AP одновременно");
      break;
    default:
      Serial.println("Неопределенный режим работы WiFi");
      break;
  }
  return wifiMode;
}

// ************************************************************* ChipWiFi.h ***
