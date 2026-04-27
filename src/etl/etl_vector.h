#ifndef ETL_VECTOR_H
#define ETL_VECTOR_H

#include "etl_platform.h"

namespace etl {

template <typename T, size_t MAX_SIZE>
class vector {
public:
    typedef T        value_type;
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    // Construction ──────────────────────────────────────────────────────
    vector() : _size(0) {
        memset(_data, 0, sizeof(_data));
    }

    vector(size_type count, const T& value) : _size(0) {
        assign(count, value);
    }

    // Element access ────────────────────────────────────────────────────
    reference operator[](size_type i) {
        ETL_ASSERT(i < _size);
        return _data[i];
    }

    const_reference operator[](size_type i) const {
        ETL_ASSERT(i < _size);
        return _data[i];
    }

    reference at(size_type i) {
        ETL_ASSERT(i < _size);
        return _data[i];
    }

    reference front() { ETL_ASSERT(_size > 0); return _data[0]; }
    reference back()  { ETL_ASSERT(_size > 0); return _data[_size - 1]; }
    const_reference front() const { ETL_ASSERT(_size > 0); return _data[0]; }
    const_reference back()  const { ETL_ASSERT(_size > 0); return _data[_size - 1]; }

    T* data() { return _data; }
    const T* data() const { return _data; }

    // Iterators ─────────────────────────────────────────────────────────
    iterator begin() { return _data; }
    iterator end()   { return _data + _size; }
    const_iterator begin() const { return _data; }
    const_iterator end()   const { return _data + _size; }
    const_iterator cbegin() const { return _data; }
    const_iterator cend()   const { return _data + _size; }

    // Capacity ──────────────────────────────────────────────────────────
    bool      empty()    const { return _size == 0; }
    bool      full()     const { return _size == MAX_SIZE; }
    size_type size()     const { return _size; }
    size_type max_size() const { return MAX_SIZE; }
    size_type capacity() const { return MAX_SIZE; }
    size_type available()const { return MAX_SIZE - _size; }

    // Modifiers ─────────────────────────────────────────────────────────
    void push_back(const T& value) {
        ETL_ASSERT(!full());
        _data[_size++] = value;
    }

    void pop_back() {
        ETL_ASSERT(!empty());
        --_size;
    }

    void clear() { _size = 0; }

    iterator insert(iterator pos, const T& value) {
        ETL_ASSERT(!full());
        ETL_ASSERT(pos >= begin() && pos <= end());
        // Shift elements right
        for (iterator it = end(); it != pos; --it) *it = *(it - 1);
        *pos = value;
        ++_size;
        return pos;
    }

    iterator erase(iterator pos) {
        ETL_ASSERT(pos >= begin() && pos < end());
        for (iterator it = pos; it != end() - 1; ++it) *it = *(it + 1);
        --_size;
        return pos;
    }

    void resize(size_type new_size, const T& value = T()) {
        ETL_ASSERT(new_size <= MAX_SIZE);
        if (new_size > _size) {
            for (size_type i = _size; i < new_size; ++i) _data[i] = value;
        }
        _size = new_size;
    }

    void assign(size_type count, const T& value) {
        ETL_ASSERT(count <= MAX_SIZE);
        for (size_type i = 0; i < count; ++i) _data[i] = value;
        _size = count;
    }

    // ── Search helpers ────────────────────────────────────────────────────
    bool contains(const T& value) const {
        for (size_type i = 0; i < _size; ++i)
            if (_data[i] == value) return true;
        return false;
    }

    iterator find(const T& value) {
        for (iterator it = begin(); it != end(); ++it)
            if (*it == value) return it;
        return end();
    }

private:
    T         _data[MAX_SIZE];
    size_type _size;
};

}

#endif