#ifndef ETL_STATE_MACHINE_H
#define ETL_STATE_MACHINE_H

/*
 * etl_state_machine.h
 * Finite State Machine (FSM) for Arduino/ESP32.
 * No heap, no virtual dispatch overhead beyond what you define.
 *
 * Usage pattern:
 *   1. Define an enum of states
 *   2. Define an enum of events
 *   3. Build a transition table
 *   4. Attach optional enter/exit callbacks per state
 *
 * Example (traffic light):
 *   etl::state_machine<TrafficState, TrafficEvent, 3, 4> fsm;
 *   fsm.add_transition(STATE_RED,    EVENT_TIMER, STATE_GREEN);
 *   fsm.add_transition(STATE_GREEN,  EVENT_TIMER, STATE_YELLOW);
 *   fsm.add_transition(STATE_YELLOW, EVENT_TIMER, STATE_RED);
 *   fsm.set_on_enter(STATE_RED, []() { Serial.println("RED"); });
 *   fsm.start(STATE_RED);
 *   fsm.process_event(EVENT_TIMER);  // → GREEN
 */

#include "etl_platform.h"

namespace etl {

// Callback type: plain function pointer (no std::function, no heap)
typedef void (*fsm_callback)();

template <typename TState, typename TEvent,
          size_t MAX_STATES, size_t MAX_TRANSITIONS>
class state_machine {
public:

    struct transition {
        TState     from;
        TEvent     event;
        TState     to;
        fsm_callback action;   // optional. Called during transition
    };

    struct state_callbacks {
        TState       state;
        fsm_callback on_enter;
        fsm_callback on_exit;
        fsm_callback on_update; // called every tick() while in this state
    };

    state_machine()
        : _current_state(TState(0)), _started(false),
          _trans_count(0), _state_count(0),
          _on_any_transition(nullptr) {}

    // Configuration ─────────────────────────────────────────────────────

    /**
     * State transition.
     * @param from    Source state
     * @param event   Triggering event
     * @param to      Destination state
     * @param action  Optional callback fired during this specific transition
     */
    bool add_transition(TState from, TEvent event, TState to,
                        fsm_callback action = nullptr) {
        if (_trans_count >= MAX_TRANSITIONS) return false;
        _transitions[_trans_count++] = { from, event, to, action };
        return true;
    }

    bool set_callbacks(TState state,
                       fsm_callback on_enter  = nullptr,
                       fsm_callback on_exit   = nullptr,
                       fsm_callback on_update = nullptr) {
        // Update existing
        for (size_t i = 0; i < _state_count; ++i) {
            if (_states[i].state == state) {
                _states[i].on_enter  = on_enter;
                _states[i].on_exit   = on_exit;
                _states[i].on_update = on_update;
                return true;
            }
        }
        if (_state_count >= MAX_STATES) return false;
        _states[_state_count++] = { state, on_enter, on_exit, on_update };
        return true;
    }

    // Convenience single-callback setters
    bool set_on_enter (TState s, fsm_callback cb) { return _set_cb(s, cb, 0); }
    bool set_on_exit  (TState s, fsm_callback cb) { return _set_cb(s, cb, 1); }
    bool set_on_update(TState s, fsm_callback cb) { return _set_cb(s, cb, 2); }

    /** Callback fired on every successful transition regardless of state. */
    void set_on_any_transition(fsm_callback cb) { _on_any_transition = cb; }

    // Runtime ───────────────────────────────────────────────────────────

    void start(TState initial) {
        _current_state = initial;
        _started = true;
        _call_enter(initial);
    }

    bool process_event(TEvent event) {
        if (!_started) return false;
        for (size_t i = 0; i < _trans_count; ++i) {
            transition& t = _transitions[i];
            if (t.from == _current_state && t.event == event) {
                _call_exit(_current_state);
                if (t.action) t.action();
                if (_on_any_transition) _on_any_transition();
                _current_state = t.to;
                _call_enter(_current_state);
                return true;
            }
        }
        return false; // no matching transition
    }

    void tick() {
        if (!_started) return;
        _call_update(_current_state);
    }

    // Query ─────────────────────────────────────────────────────────────
    TState current_state() const { return _current_state; }
    bool   is_in(TState s) const { return _started && _current_state == s; }
    bool   started()       const { return _started; }

    /** Force a state change without firing transition callbacks. */
    void force_state(TState s) {
        _current_state = s;
        _started = true;
    }

private:
    TState       _current_state;
    bool         _started;
    transition   _transitions[MAX_TRANSITIONS];
    size_t       _trans_count;
    state_callbacks _states[MAX_STATES];
    size_t       _state_count;
    fsm_callback _on_any_transition;

    state_callbacks* _find_state(TState s) {
        for (size_t i = 0; i < _state_count; ++i)
            if (_states[i].state == s) return &_states[i];
        return nullptr;
    }

    void _call_enter (TState s) { auto* sc = _find_state(s); if (sc && sc->on_enter)  sc->on_enter();  }
    void _call_exit  (TState s) { auto* sc = _find_state(s); if (sc && sc->on_exit)   sc->on_exit();   }
    void _call_update(TState s) { auto* sc = _find_state(s); if (sc && sc->on_update) sc->on_update(); }

    bool _set_cb(TState s, fsm_callback cb, int which) {
        for (size_t i = 0; i < _state_count; ++i) {
            if (_states[i].state == s) {
                if (which == 0) _states[i].on_enter  = cb;
                if (which == 1) _states[i].on_exit   = cb;
                if (which == 2) _states[i].on_update = cb;
                return true;
            }
        }
        if (_state_count >= MAX_STATES) return false;
        state_callbacks sc = { s, nullptr, nullptr, nullptr };
        if (which == 0) sc.on_enter  = cb;
        if (which == 1) sc.on_exit   = cb;
        if (which == 2) sc.on_update = cb;
        _states[_state_count++] = sc;
        return true;
    }
};
} 
#endif
