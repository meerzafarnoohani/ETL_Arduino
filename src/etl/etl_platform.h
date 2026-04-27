#ifndef ETL_ARDUINO_PLATFORM_H
#define ETL_ARDUINO_PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

// Compiler / architecture detection ──────────────────────────────────────
#if defined(ESP32)
  #define ETL_TARGET_ESP32
  #define ETL_PLATFORM "ESP32"
#elif defined(ESP8266)
  #define ETL_TARGET_ESP8266
  #define ETL_PLATFORM "ESP8266"
#elif defined(ARDUINO_ARCH_SAMD)
  #define ETL_TARGET_SAMD
  #define ETL_PLATFORM "SAMD"
#elif defined(ARDUINO_ARCH_RP2040)
  #define ETL_TARGET_RP2040
  #define ETL_PLATFORM "RP2040"
#elif defined(__AVR__)
  #define ETL_TARGET_AVR
  #define ETL_PLATFORM "AVR"
#else
  #define ETL_PLATFORM "Unknown"
#endif

// Core ETL settings ───────────────────────────────────────────────────────
// No exceptions on embedded targets
#define ETL_NO_EXCEPTIONS

// No dynamic memory. All containers are fixed-size
#define ETL_NO_DYNAMIC_MEMORY

// Use assert() for error checking (can be overridden)
#ifndef ETL_ASSERT
  #define ETL_ASSERT(expr) assert(expr)
#endif

// Silence unused variable warnings
#define ETL_UNUSED(x) (void)(x)

// Type aliases ────────────────────────────────────────────────────────────
typedef uint8_t  etl_uint8;
typedef uint16_t etl_uint16;
typedef uint32_t etl_uint32;
typedef int8_t   etl_int8;
typedef int16_t  etl_int16;
typedef int32_t  etl_int32;
typedef size_t   etl_size;

#endif
