/** Arduino, ESP32, C/C++ ******************************************* jpr.h ***
 * 
 *                      Объявить/проинициализировать общепрограммные переменные
 *                                                     
 * v1.0.0, 24.01.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 24.01.2026
**/

#pragma once     

//#include "inimem.h"

#define jpr(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.print(buffer); \
  }

#define jprln(format, ...) \
  { \
    char buffer[256]; \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
    Serial.println(buffer); \
  }

// ****************************************************************************
// *              Показать состояние памяти с заданным префиксом              *
// ****************************************************************************

/**
 * xPortGetCoreID()        - функция возвращает номер ядра, на котором выполняется текущая задача
 * uxTaskPriorityGet(NULL) - возвращает приоритет текущей задачи (задачи, из которой была вызвана функция)
 * ESP.getFreeHeap()       - возвращает размер свободной кучи (heap) в байтах
 * ESP.getHeapSize()       - возвращает полный размер внутренней оперативной памяти в байтах (ОЗУ)
 * ESP.getFreePsram()      - свободный объём внешней оперативной памяти PSRAM
 * ESP.getPsramSize()      - полный объём внешней оперативной памяти PSRAM
**/
void print_mem(const char* text) 
{
  jprln("[%s] ядро: %d, приоритет: %d, свободная куча %6d от ОЗУ %6d, FreePSRAM %6d от FLASH %6d", text, xPortGetCoreID(), uxTaskPriorityGet(NULL), ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getFreePsram(), ESP.getPsramSize() );
}

// ****************************************************************** jpr.h ***
