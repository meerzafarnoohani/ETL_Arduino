/*
 * BasicVector.ino
 * ETL Arduino — Vector, Queue & Map Demo
 *
 * Demonstrates all three fixed-size containers with zero heap allocation.
 * Open Serial Monitor at 115200 baud.
 */

#include <ETL_Arduino.h>

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println(F("\n=== ETL Arduino — Containers Demo ===\n"));

    // ── VECTOR ──────────────────────────────────────────────────────────
    Serial.println(F("--- etl::vector<float, 8> ---"));
    etl::vector<float, 8> temps;

    temps.push_back(23.5f);
    temps.push_back(24.1f);
    temps.push_back(22.8f);
    temps.push_back(25.0f);

    Serial.print(F("Size: ")); Serial.println(temps.size());
    Serial.print(F("Full: ")); Serial.println(temps.full() ? "yes" : "no");

    // Range-based for loop works
    float sum = 0;
    for (auto& t : temps) sum += t;
    Serial.print(F("Average temp: "));
    Serial.println(sum / temps.size(), 2);

    // Search
    if (temps.contains(25.0f)) Serial.println(F("Found 25.0 in vector"));

    // Erase element at index 1
    temps.erase(temps.begin() + 1);
    Serial.print(F("After erase, size: ")); Serial.println(temps.size());
    Serial.println();

    // ── QUEUE ────────────────────────────────────────────────────────────
    Serial.println(F("--- etl::queue<int, 6> (FIFO) ---"));
    etl::queue<int, 6> q;

    q.push(10); q.push(20); q.push(30); q.push(40);
    Serial.print(F("Queue size: ")); Serial.println(q.size());
    Serial.print(F("Front: ")); Serial.println(q.front());

    while (!q.empty()) {
        Serial.print(F("Dequeue: ")); Serial.println(q.front());
        q.pop();
    }
    Serial.println();

    // ── MAP ──────────────────────────────────────────────────────────────
    Serial.println(F("--- etl::map<etl::string, float, 8> ---"));

    // Using etl::string as key
    etl::map<etl::string<16>, float, 8> sensors;
    sensors.insert(etl::string<16>("temperature"), 24.5f);
    sensors.insert(etl::string<16>("humidity"),    62.3f);
    sensors.insert(etl::string<16>("pressure"),    1013.2f);

    Serial.print(F("Map size: ")); Serial.println(sensors.size());

    etl::string<16> key("humidity");
    if (sensors.contains(key)) {
        Serial.print(F("Humidity: "));
        Serial.println(sensors[key]);
    }

    // Iterate all entries
    for (auto& entry : sensors) {
        Serial.print(entry.key.c_str());
        Serial.print(F(" = "));
        Serial.println(entry.value);
    }
    Serial.println();

    // ── Memory safety check ───────────────────────────────────────────────
    Serial.println(F("--- Memory check ---"));
    Serial.print(F("Free heap: "));
    #ifdef ESP32
        Serial.print(ESP.getFreeHeap());
        Serial.println(F(" bytes (should be unchanged — no heap used)"));
    #else
        Serial.println(F("(ESP32 only)"));
    #endif
}

void loop() {
    delay(5000);
}
