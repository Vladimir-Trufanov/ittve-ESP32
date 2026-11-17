/*
USP32-S3-USB-OTG
Partitions Info: 4MB со SPIFFS (1.2 APP, 1.5 SPIFFS)
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------

Partitions Info: 8MB со SPIFFS (3 APP, 1.5 SPIFFS)
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  3264.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00340000, size:  3264.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00670000, size:  1536.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x007F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Скетч использует 306823 байт (9%) памяти устройства. Всего доступно 3342336 байт.
Глобальные переменные используют 21484 байт (6%) динамической памяти, оставляя 306196 байт для локальных переменных. Максимум: 327680 байт.

================================================================================
ESP32S3 Dev Module (OPI PSRAM)
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------


CUSTOM
Скетч использует 307881 байт (1%) памяти устройства. Всего доступно 16777216 байт.
Глобальные переменные используют 21916 байт (6%) динамической памяти, оставляя 305764 байт для локальных переменных. Максимум: 327680 байт.

Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  3072.0 KB, type:  APP, subtype: OTA_0
             spiffs : addr: 0x00310000, size:   896.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
*/

/*
Распределение "ESP32S3_Dev_Module_20261116.csv"

=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-S3
  Package           : 0
  Revision          : 0.02
  Cores             : 2
  CPU Frequency     : 240 MHz
  XTAL Frequency    : 40 MHz
  Features Bitfield : 0x00000012
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : No
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   344768 B ( 336.7 KB)
  Free Bytes        :   312292 B ( 305.0 KB)
  Allocated Bytes   :    28080 B (  27.4 KB)
  Minimum Free Bytes:   307144 B ( 299.9 KB)
  Largest Free Block:   286708 B ( 280.0 KB)
------------------------------------------
SPIRAM Memory Info:
------------------------------------------
  Total Size        :  8388608 B (8192.0 KB)
  Free Bytes        :  8386096 B (8189.5 KB)
  Allocated Bytes   :      136 B (   0.1 KB)
  Minimum Free Bytes:  8386096 B (8189.5 KB)
  Largest Free Block:  8257524 B (8064.0 KB)
  Bus Mode          : OPI
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         : 16777216B (16 MB)
  Block Size        :    65536B (  64.0 KB)
  Sector Size       :     4096B (   4.0 KB)
  Page Size         :      256B (   0.2 KB)
  Bus Speed         : 80 MHz
  Flash Frequency   : 80 MHz (source: 160 MHz, divider: 2)
  Bus Mode          : QIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  3072.0 KB, type:  APP, subtype: OTA_0
             spiffs : addr: 0x00310000, size:   896.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------

*/

/*
 * NodeMCU_32S
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32
  Package           : D0WD-Q6
  Revision          : 1.00
  Cores             : 2
  CPU Frequency     : 240 MHz
  XTAL Frequency    : 40 MHz
  Features Bitfield : 0x00000032
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : Yes
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   377732 B ( 368.9 KB)
  Free Bytes        :   337024 B ( 329.1 KB)
  Allocated Bytes   :    33528 B (  32.7 KB)
  Minimum Free Bytes:   331244 B ( 323.5 KB)
  Largest Free Block:   110580 B ( 108.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304B (4 MB)
  Block Size        :    65536B (  64.0 KB)
  Sector Size       :     4096B (   4.0 KB)
  Page Size         :      256B (   0.2 KB)
  Bus Speed         : 80 MHz
  Flash Frequency   : 80 MHz (source: 80 MHz, divider: 1)
  Bus Mode          : DIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  3072.0 KB, type:  APP, subtype: OTA_0
             spiffs : addr: 0x00310000, size:   896.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Nov 17 2026 20:37:12
  Compile Host OS   : windows
  ESP-IDF Version   : v5.5.4
  Arduino Version   : 3.3.8
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : NODEMCU_32S
  Arduino Variant   : nodemcu-32s
  Arduino FQBN      : esp32:esp32:nodemcu-32s:FlashFreq=80,UploadSpeed=115200,DebugLevel=verbose,EraseFlash=all
============ Before Setup End ============
*/

void saymem()
{
 Serial.print("Cвободная куча:  "); Serial.print(ESP.getFreeHeap());  Serial.print(" от ОЗУ   "); Serial.println(ESP.getHeapSize());
 Serial.print("FreePSRAM:      ");  Serial.print(ESP.getFreePsram()); Serial.print(" от FLASH "); Serial.println(ESP.getPsramSize());
}

void setup() 
{
  Serial.begin(115200);
}

uint32_t tTime=millis();

void loop() 
{
  tTime=millis(); 
  Serial.print("\nВремя:          "); Serial.println(tTime);
  saymem();
  delay(3015);
}
