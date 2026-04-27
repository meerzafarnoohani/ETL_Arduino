#ifndef ETL_MAP_H
#define ETL_MAP_H

#include "etl_platform.h"

namespace etl {

template <typename TKey, typename TValue, size_t MAX_SIZE>
class map {
public:
    struct pair {
        TKey   key;
        TValue value;
    };

    typedef pair*       iterator;
    typedef const pair* const_iterator;
    typedef size_t      size_type;

    map() : _size(0) {}

    // Capacity ──────────────────────────────────────────────────────────
    bool      empty()    const { return _size == 0; }
    bool      full()     const { return _size == MAX_SIZE; }
    size_type size()     const { return _size; }
    size_type max_size() const { return MAX_SIZE; }

    // Iterators ─────────────────────────────────────────────────────────
    iterator begin() { return _data; }
    iterator end()   { return _data + _size; }
    const_iterator begin() const { return _data; }
    const_iterator end()   const { return _data + _size; }

    // Lookup ────────────────────────────────────────────────────────────
    iterator find(const TKey& key) {
        for (size_type i = 0; i < _size; ++i)
            if (_data[i].key == key) return &_data[i];
        return end();
    }

    const_iterator find(const TKey& key) const {
        for (size_type i = 0; i < _size; ++i)
            if (_data[i].key == key) return &_data[i];
        return end();
    }

    bool contains(const TKey& key) const {
        return find(key) != end();
    }

    // Element access ────────────────────────────────────────────────────
    TValue& operator[](const TKey& key) {
        iterator it = find(key);
        if (it != end()) return it->value;
        // Insert default
        ETL_ASSERT(!full());
        _data[_size].key   = key;
        _data[_size].value = TValue();
        return _data[_size++].value;
    }

    TValue& at(const TKey& key) {
        iterator it = find(key);
        ETL_ASSERT(it != end());
        return it->value;
    }

    // Modifiers ─────────────────────────────────────────────────────────
    bool insert(const TKey& key, const TValue& value) {
        if (contains(key)) {
            find(key)->value = value; // update existing
            return true;
        }
        if (full()) return false;
        _data[_size].key   = key;
        _data[_size].value = value;
        ++_size;
        return true;
    }

    bool erase(const TKey& key) {
        for (size_type i = 0; i < _size; ++i) {
            if (_data[i].key == key) {
                // Shift left
                for (size_type j = i; j < _size - 1; ++j)
                    _data[j] = _data[j + 1];
                --_size;
                return true;
            }
        }
        return false;
    }

    void clear() { _size = 0; }

private:
    pair      _data[MAX_SIZE];
    size_type _size;
};

}

#endif
