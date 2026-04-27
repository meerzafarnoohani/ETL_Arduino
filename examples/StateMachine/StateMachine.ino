/*
 * StateMachine.ino
 * ETL Arduino — Finite State Machine Demo
 *
 * Models a traffic light using etl::state_machine.
 * States:  RED → GREEN → YELLOW → RED ...
 * Events:  TIMER (fires every N seconds)
 *
 * Open Serial Monitor at 115200 baud.
 */

#include <ETL_Arduino.h>

// ── State & Event definitions ────────────────────────────────────────────────
enum TrafficState { STATE_RED, STATE_GREEN, STATE_YELLOW };
enum TrafficEvent { EVENT_TIMER, EVENT_EMERGENCY };

// 3 states, up to 6 transitions
etl::state_machine<TrafficState, TrafficEvent, 3, 6> trafficLight;

unsigned long lastEvent = 0;

// ── State enter callbacks ────────────────────────────────────────────────────
void onEnterRed()    { Serial.println(F("🔴 RED    — Stop!")); }
void onEnterGreen()  { Serial.println(F("🟢 GREEN  — Go!"));  }
void onEnterYellow() { Serial.println(F("🟡 YELLOW — Slow down...")); }
void onEnterEmergency() { Serial.println(F("🚨 EMERGENCY — All stop!")); }

void onExitGreen()   { Serial.println(F("   (Leaving green)")); }

// ── Transition action ────────────────────────────────────────────────────────
void onTransition()  { Serial.println(F("   [transition fired]")); }

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println(F("\n=== ETL Arduino — State Machine Demo ===\n"));

    // Add transitions
    trafficLight.add_transition(STATE_RED,    EVENT_TIMER,     STATE_GREEN);
    trafficLight.add_transition(STATE_GREEN,  EVENT_TIMER,     STATE_YELLOW);
    trafficLight.add_transition(STATE_YELLOW, EVENT_TIMER,     STATE_RED);
    trafficLight.add_transition(STATE_GREEN,  EVENT_EMERGENCY, STATE_RED);

    // Attach callbacks
    trafficLight.set_on_enter(STATE_RED,    onEnterRed);
    trafficLight.set_on_enter(STATE_GREEN,  onEnterGreen);
    trafficLight.set_on_enter(STATE_YELLOW, onEnterYellow);
    trafficLight.set_on_exit (STATE_GREEN,  onExitGreen);
    trafficLight.set_on_any_transition(onTransition);

    // Start FSM
    trafficLight.start(STATE_RED);
    lastEvent = millis();
}

void loop() {
    // Tick every 3 seconds
    if (millis() - lastEvent >= 3000) {
        lastEvent = millis();
        Serial.print(F("\n[Timer tick] Current state: "));
        Serial.println(trafficLight.current_state());

        trafficLight.process_event(EVENT_TIMER);
    }

    // Optional: call tick() every loop for on_update callbacks
    trafficLight.tick();
}
