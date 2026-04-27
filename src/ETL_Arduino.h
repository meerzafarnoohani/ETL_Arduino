#ifndef ETL_ARDUINO_H
#define ETL_ARDUINO_H

/*
 * ETL_Arduino.h
 * Embedded Template Library Arduino/ESP32 Port
 * by Meer Zafarullah Noohani
 *
 * A production-grade, zero-heap alternative to the C++ STL,
 * designed specifically for microcontrollers.
 *
 * Include this single header to get everything:
 *   #include <ETL_Arduino.h>
 *
 * Or include individual modules as needed:
 *   #include <etl/etl_vector.h>
 *   #include <etl/etl_queue.h>
 *   #include <etl/etl_map.h>
 *   #include <etl/etl_string.h>
 *   #include <etl/etl_state_machine.h>
 *
 * ── Available containers ────────────────────────────────────────────────
 *
 *  etl::vector<T, N>              Fixed-size dynamic array
 *  etl::queue<T, N>               Fixed-size FIFO (circular buffer)
 *  etl::map<TKey, TVal, N>        Fixed-size key-value store
 *  etl::string<N>                 Fixed-capacity string (no heap)
 *  etl::state_machine<S,E,NS,NT>  Finite State Machine with callbacks
 *
 * ── Memory model ────────────────────────────────────────────────────────
 *
 *  All storage is allocated at compile time (stack or global).
 *  No malloc(), no new, no heap fragmentation.
 *  Safe for use in ISRs and FreeRTOS tasks on ESP32.
 *
 * ── Quick reference ─────────────────────────────────────────────────────
 *
 *  etl::vector<int, 16> v;
 *  v.push_back(42);
 *  Serial.println(v[0]);
 *
 *  etl::queue<float, 8> q;
 *  q.push(3.14f);
 *  float x = q.front(); q.pop();
 *
 *  etl::map<const char*, int, 8> m;
 *  m.insert("temp", 25);
 *  Serial.println(m["temp"]);
 *
 *  etl::string<64> s("Hello ");
 *  s += "ESP32";
 *  Serial.println(s.c_str());
 *
 *  // State machine — see StateMachine example for full usage
 *  etl::state_machine<MyState, MyEvent, 4, 8> fsm;
 */

#include "etl/etl_platform.h"
#include "etl/etl_vector.h"
#include "etl/etl_queue.h"
#include "etl/etl_map.h"
#include "etl/etl_string.h"
#include "etl/etl_state_machine.h"

#endif // ETL_ARDUINO_H
