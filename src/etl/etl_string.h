#ifndef ETL_STRING_H
#define ETL_STRING_H

#include "etl_platform.h"

namespace etl {

template <size_t MAX_LEN>
class string {
public:
    typedef char*       iterator;
    typedef const char* const_iterator;
    typedef size_t      size_type;

    static const size_type npos = (size_type)-1;

    // Construction ──────────────────────────────────────────────────────
    string() { _buf[0] = '\0'; _len = 0; }

    string(const char* s) {
        _len = 0;
        assign(s);
    }

    string(const string& other) {
        assign(other._buf);
    }

    // Assignment ────────────────────────────────────────────────────────
    string& operator=(const char* s)    { assign(s); return *this; }
    string& operator=(const string& s)  { assign(s._buf); return *this; }

    void assign(const char* s) {
        if (!s) { clear(); return; }
        _len = 0;
        while (s[_len] && _len < MAX_LEN) {
            _buf[_len] = s[_len];
            ++_len;
        }
        _buf[_len] = '\0';
    }

    // Capacity ──────────────────────────────────────────────────────────
    size_type length()   const { return _len; }
    size_type size()     const { return _len; }
    size_type max_size() const { return MAX_LEN; }
    size_type available()const { return MAX_LEN - _len; }
    bool      empty()    const { return _len == 0; }
    bool      full()     const { return _len == MAX_LEN; }

    // Access ────────────────────────────────────────────────────────────
    const char* c_str() const { return _buf; }
    char* data()              { return _buf; }

    char& operator[](size_type i)       { return _buf[i]; }
    char  operator[](size_type i) const { return _buf[i]; }

    char front() const { return _buf[0]; }
    char back()  const { return _len > 0 ? _buf[_len - 1] : '\0'; }

    // Iterators ─────────────────────────────────────────────────────────
    iterator begin() { return _buf; }
    iterator end()   { return _buf + _len; }
    const_iterator begin() const { return _buf; }
    const_iterator end()   const { return _buf + _len; }

    // Modifiers ─────────────────────────────────────────────────────────
    void clear() { _buf[0] = '\0'; _len = 0; }

    string& operator+=(const char* s) { append(s); return *this; }
    string& operator+=(char c)        { push_back(c); return *this; }
    string& operator+=(const string& s) { append(s._buf); return *this; }

    void append(const char* s) {
        if (!s) return;
        size_type i = 0;
        while (s[i] && _len < MAX_LEN) {
            _buf[_len++] = s[i++];
        }
        _buf[_len] = '\0';
    }

    void push_back(char c) {
        if (_len < MAX_LEN) {
            _buf[_len++] = c;
            _buf[_len]   = '\0';
        }
    }

    void pop_back() {
        if (_len > 0) _buf[--_len] = '\0';
    }

    // Comparison ────────────────────────────────────────────────────────
    bool operator==(const char* s)    const { return strcmp(_buf, s) == 0; }
    bool operator==(const string& s)  const { return strcmp(_buf, s._buf) == 0; }
    bool operator!=(const char* s)    const { return !(*this == s); }
    bool operator!=(const string& s)  const { return !(*this == s); }
    bool operator< (const string& s)  const { return strcmp(_buf, s._buf) < 0; }

    // ── Search ────────────────────────────────────────────────────────────
    size_type find(char c, size_type from = 0) const {
        for (size_type i = from; i < _len; ++i)
            if (_buf[i] == c) return i;
        return npos;
    }

    size_type find(const char* s, size_type from = 0) const {
        const char* p = strstr(_buf + from, s);
        return p ? (size_type)(p - _buf) : npos;
    }

    bool starts_with(const char* s) const {
        return strncmp(_buf, s, strlen(s)) == 0;
    }

    bool ends_with(const char* s) const {
        size_type sl = strlen(s);
        if (sl > _len) return false;
        return strcmp(_buf + _len - sl, s) == 0;
    }

    bool contains(const char* s) const { return find(s) != npos; }

    // Transformation ────────────────────────────────────────────────────
    void to_upper() {
        for (size_type i = 0; i < _len; ++i)
            if (_buf[i] >= 'a' && _buf[i] <= 'z') _buf[i] -= 32;
    }

    void to_lower() {
        for (size_type i = 0; i < _len; ++i)
            if (_buf[i] >= 'A' && _buf[i] <= 'Z') _buf[i] += 32;
    }

    void trim() {
        // Trim trailing
        while (_len > 0 && (_buf[_len-1] == ' ' || _buf[_len-1] == '\t'
               || _buf[_len-1] == '\n' || _buf[_len-1] == '\r')) {
            _buf[--_len] = '\0';
        }
        // Trim leading
        size_type start = 0;
        while (start < _len && (_buf[start] == ' ' || _buf[start] == '\t')) ++start;
        if (start > 0) {
            memmove(_buf, _buf + start, _len - start + 1);
            _len -= start;
        }
    }

    string substr(size_type pos, size_type len = npos) const {
        string result;
        if (pos >= _len) return result;
        size_type actual = (len == npos || pos + len > _len) ? _len - pos : len;
        for (size_type i = 0; i < actual && i < MAX_LEN; ++i)
            result._buf[i] = _buf[pos + i];
        result._len = actual;
        result._buf[actual] = '\0';
        return result;
    }

    // Numeric conversion ────────────────────────────────────────────────
    int   to_int()   const { return atoi(_buf); }
    float to_float() const { return atof(_buf); }

    void from_int(int v) {
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%d", v);
        assign(tmp);
    }

    void from_float(float v, int decimals = 2) {
        char tmp[24];
        snprintf(tmp, sizeof(tmp), "%.*f", decimals, v);
        assign(tmp);
    }

private:
    char      _buf[MAX_LEN + 1]; // +1 for null terminator
    size_type _len;
};

} 

#endif
