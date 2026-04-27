/*
 * StringHandling.ino
 * ETL Arduino — etl::string Demo
 *
 * Shows safe string handling with zero heap allocation.
 * Great for parsing serial commands, building sensor payloads,
 * formatting display output — all without String fragmentation.
 *
 * Open Serial Monitor at 115200 baud.
 */

#include <ETL_Arduino.h>

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println(F("\n=== ETL Arduino — String Demo ===\n"));

    // ── Basic usage ───────────────────────────────────────────────────────
    etl::string<64> s("Hello");
    s += ", ESP32!";
    Serial.print(F("String: ")); Serial.println(s.c_str());
    Serial.print(F("Length: ")); Serial.println(s.length());
    Serial.print(F("Available: ")); Serial.println(s.available());

    // ── Case conversion ───────────────────────────────────────────────────
    etl::string<32> upper("hello world");
    upper.to_upper();
    Serial.print(F("Upper: ")); Serial.println(upper.c_str());

    etl::string<32> lower("ARDUINO ESP32");
    lower.to_lower();
    Serial.print(F("Lower: ")); Serial.println(lower.c_str());

    // ── Search ────────────────────────────────────────────────────────────
    etl::string<64> msg("sensor:temp:24.5");
    Serial.print(F("Contains 'temp': "));
    Serial.println(msg.contains("temp") ? "yes" : "no");

    size_t pos = msg.find(':');
    Serial.print(F("First ':' at pos: ")); Serial.println(pos);

    // ── Substring extraction ──────────────────────────────────────────────
    etl::string<32> key   = msg.substr(0, 6);   // "sensor"
    etl::string<32> value = msg.substr(12);      // "24.5"
    Serial.print(F("Key:   ")); Serial.println(key.c_str());
    Serial.print(F("Value: ")); Serial.println(value.c_str());

    // ── Numeric conversion ────────────────────────────────────────────────
    float temp = value.to_float();
    Serial.print(F("Parsed float: ")); Serial.println(temp, 1);

    etl::string<16> formatted;
    formatted.from_float(temp * 1.8f + 32, 1); // Celsius to Fahrenheit
    Serial.print(F("Fahrenheit: ")); Serial.println(formatted.c_str());

    // ── Trim ──────────────────────────────────────────────────────────────
    etl::string<32> padded("  trim me!  ");
    padded.trim();
    Serial.print(F("Trimmed: '")); Serial.print(padded.c_str()); Serial.println(F("'"));

    // ── Building a JSON-like payload (no heap!) ───────────────────────────
    Serial.println(F("\n--- Building sensor payload ---"));
    etl::string<128> payload("{");
    payload += "\"temp\":";
    etl::string<8> t; t.from_float(24.5f, 1);
    payload += t;
    payload += ",\"humid\":";
    etl::string<8> h; h.from_float(62.3f, 1);
    payload += h;
    payload += "}";
    Serial.println(payload.c_str());
    Serial.print(F("Payload length: ")); Serial.println(payload.length());
}

void loop() {
    delay(5000);
}
