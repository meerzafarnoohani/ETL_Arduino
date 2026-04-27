# ETL (Embedded Template Library) Arduino

> Fixed-size containers and state machines for Arduino & ESP32 **zero heap allocation, zero fragmentation.**

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue?logo=arduino)](https://github.com/meerzafarnoohani/ETL_Arduino)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32%20%7C%20Arduino-orange)](https://github.com/meerzafarnoohani/ETL_Arduino)

---

## Why ETL?

Arduino's `String` class and STL containers (`std::vector`, `std::map`) use **dynamic memory allocation** (heap). On microcontrollers this causes:

- 🔴 **Heap fragmentation** — memory gets swiss-cheesed over time
- 🔴 **Unpredictable crashes** — allocation fails silently
- 🔴 **Hard to debug** — symptoms appear far from the root cause

ETL_Arduino fixes this by making the **capacity a compile-time constant**. All memory lives on the stack or in static storage — no `malloc`, no `new`, no surprises.

---

## Features

| Container | Description |
|-----------|-------------|
| `etl::vector<T, N>` | Dynamic array with fixed max capacity |
| `etl::queue<T, N>` | FIFO queue backed by a circular buffer |
| `etl::map<K, V, N>` | Key-value store, no dynamic allocation |
| `etl::string<N>` | Safe string class, replaces Arduino `String` |
| `etl::state_machine<S,E,NS,NT>` | FSM with enter/exit/update callbacks |

---

## Installation

### Via Arduino IDE
1. Download `ETL_Arduino.zip` from the [Releases](https://github.com/meerzafarnoohani/ETL_Arduino/releases) page
2. **Sketch → Include Library → Add .ZIP Library...**
3. Select the zip

### Via Arduino Library Manager
Search for **ETL_Arduino** in **Tools → Manage Libraries**.

### Manual
```bash
cd ~/Arduino/libraries
git clone https://github.com/meerzafarnoohani/ETL_Arduino.git
```

---

## Quick Start

```cpp
#include <ETL_Arduino.h>
```

Or include only what you need:
```cpp
#include <etl/etl_vector.h>
#include <etl/etl_queue.h>
#include <etl/etl_map.h>
#include <etl/etl_string.h>
#include <etl/etl_state_machine.h>
```

---

## API Reference

### `etl::vector<T, N>`
```cpp
etl::vector<float, 16> v;
v.push_back(3.14f);          // add element
v.pop_back();                // remove last
v[0];                        // access by index
v.size();                    // current count
v.full();                    // true if at capacity
v.contains(3.14f);           // linear search
v.erase(v.begin() + 1);      // remove by iterator
for (auto& x : v) { ... }   // range-based for loop
```

### `etl::queue<T, N>`
```cpp
etl::queue<int, 8> q;
q.push(42);       // enqueue
q.front();        // peek at front
q.pop();          // dequeue
q.empty();        // true if empty
q.full();         // true if at capacity
q.size();         // current count
```

### `etl::map<TKey, TValue, N>`
```cpp
etl::map<etl::string<16>, int, 8> m;
m.insert("temp", 25);        // add/update
m["temp"];                   // access by key (inserts default if missing)
m.at("temp");                // access (asserts if missing)
m.contains("temp");          // check existence
m.erase("temp");             // remove
m.size();                    // count of entries
for (auto& e : m) { e.key; e.value; }  // iterate
```

### `etl::string<N>`
```cpp
etl::string<64> s("Hello");
s += " ESP32";               // append string
s += '!';                    // append char
s.c_str();                   // const char* for Serial.println etc.
s.length();                  // current length
s.available();               // remaining capacity
s.to_upper();                // in-place uppercase
s.to_lower();                // in-place lowercase
s.trim();                    // strip leading/trailing whitespace
s.contains("ESP");           // substring check
s.find(':');                 // find char, returns index or npos
s.substr(0, 5);              // extract substring
s.to_int();                  // parse integer
s.to_float();                // parse float
s.from_int(42);              // format integer into string
s.from_float(3.14f, 2);     // format float with 2 decimal places
```

### `etl::state_machine<TState, TEvent, MAX_STATES, MAX_TRANSITIONS>`
```cpp
enum State { IDLE, RUNNING, ERROR };
enum Event { START, STOP, FAULT };

etl::state_machine<State, Event, 3, 6> fsm;

// Build transition table
fsm.add_transition(IDLE,    START, RUNNING);
fsm.add_transition(RUNNING, STOP,  IDLE);
fsm.add_transition(RUNNING, FAULT, ERROR);

// Attach callbacks (plain function pointers — no lambdas needed)
fsm.set_on_enter(RUNNING, []() { Serial.println("Running!"); });
fsm.set_on_exit (RUNNING, []() { Serial.println("Stopping..."); });
fsm.set_on_update(RUNNING, []() { /* called every tick() */ });

// Start and drive
fsm.start(IDLE);
fsm.process_event(START);       // IDLE → RUNNING
fsm.tick();                     // fires on_update for current state
fsm.current_state();            // returns RUNNING
fsm.is_in(RUNNING);             // true
```

---

## Examples

| Example | What it shows |
|---------|---------------|
| `BasicVector` | vector, queue and map with sensor data |
| `StateMachine` | Traffic light FSM with timed transitions |
| `StringHandling` | String parsing, building JSON payloads, numeric conversion |

---

## Memory Usage (ESP32, approximate per instance)

| Container | Stack memory |
|-----------|-------------|
| `vector<float, 16>` | 64 bytes |
| `queue<int, 8>` | 32 bytes + overhead |
| `map<string<16>, float, 8>` | ~176 bytes |
| `string<64>` | 65 bytes |
| `state_machine<3 states, 6 transitions>` | ~120 bytes |

All memory is allocated at **compile time** — the heap is untouched.

---

## Supported Platforms

| Platform | Status |
|----------|--------|
| ESP32 | ✅ Primary target |
| ESP8266 | ✅ Supported |
| Arduino Uno / Nano (AVR) | ✅ Use small N values |
| Arduino Mega | ✅ Supported |
| Arduino MKR / SAMD | ✅ Supported |
| Raspberry Pi Pico (RP2040) | ✅ Supported |

---

## Contributing

Contributions welcome! Ideas for future modules:
- `etl::stack<T, N>` — fixed-size LIFO stack
- `etl::set<T, N>` — fixed-size unique value set
- `etl::bitset<N>` — fixed-size bit manipulation
- `etl::pool<T, N>` — fixed object memory pool
- `etl::observer` — event/observer pattern

1. Fork the repo
2. Create a branch: `git checkout -b feature/etl-stack`
3. Commit and push
4. Open a Pull Request

---

## Credits

Inspired by the [Embedded Template Library](https://www.etlcpp.com) by John Wellbelove.
Re-implemented from scratch for direct Arduino/ESP32 IDE compatibility by **Meer Zafarullah Noohani** — [github.com/meerzafarnoohani](https://github.com/meerzafarnoohani).

---

## License

MIT — see [LICENSE](LICENSE).
