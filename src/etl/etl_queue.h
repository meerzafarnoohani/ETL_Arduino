#ifndef ETL_QUEUE_H
#define ETL_QUEUE_H

/*
 * etl_queue.h
 * Fixed-capacity FIFO queue backed by a circular buffer.
 * No heap allocation — capacity is set at compile time.
 *
 * Usage:
 *   etl::queue<int, 8> q;
 *   q.push(1); q.push(2);
 *   int v = q.front(); q.pop();
 */

#include "etl_platform.h"

namespace etl {

template <typename T, size_t MAX_SIZE>
class queue {
public:
    typedef T      value_type;
    typedef T&     reference;
    typedef const T& const_reference;
    typedef size_t size_type;

    queue() : _head(0), _tail(0), _size(0) {}

    // ── Capacity ──────────────────────────────────────────────────────────
    bool      empty()    const { return _size == 0; }
    bool      full()     const { return _size == MAX_SIZE; }
    size_type size()     const { return _size; }
    size_type max_size() const { return MAX_SIZE; }
    size_type available()const { return MAX_SIZE - _size; }

    // ── Element access ────────────────────────────────────────────────────
    reference front() {
        ETL_ASSERT(!empty());
        return _data[_head];
    }

    const_reference front() const {
        ETL_ASSERT(!empty());
        return _data[_head];
    }

    reference back() {
        ETL_ASSERT(!empty());
        return _data[(_tail == 0) ? MAX_SIZE - 1 : _tail - 1];
    }

    // ── Modifiers ─────────────────────────────────────────────────────────
    void push(const T& value) {
        ETL_ASSERT(!full());
        _data[_tail] = value;
        _tail = (_tail + 1) % MAX_SIZE;
        ++_size;
    }

    void pop() {
        ETL_ASSERT(!empty());
        _head = (_head + 1) % MAX_SIZE;
        --_size;
    }

    void clear() {
        _head = _tail = _size = 0;
    }

private:
    T         _data[MAX_SIZE];
    size_type _head;
    size_type _tail;
    size_type _size;
};

} // namespace etl

#endif // ETL_QUEUE_H
