/*
 * bigly - library for arbitrarily large numbers
 *
 * Copyright (C) 2023 Garyl Hester. All rights reserved.
 * 
 * This project lives at https://github.com/codefool/bigly
 *
 */
#include "bigly"
#include <algorithm>

namespace cflib {
bigly bigly::ONE(0x01);
bigly bigly::ZERO(0x00);

bigly::bigly() 
: _sign(POS)
, _len(0)
, _frac(0)
, _blkcnt(1)
, _size(BLOCK_SIZE)
{
    _m = new digit_t[BLOCK_SIZE];
    std::memset(_m, 0x00, BLOCK_SIZE);
}

bigly::bigly(const bigly& o) 
: bigly()
{
    _sign   = o._sign;
    _len    = o._len;
    _frac   = o._frac;
    _blkcnt = o._blkcnt;
    _size   = o._size;
    std::memcpy(_m, o._m, _size);
}

bigly::bigly(const int64_t val) 
: bigly()
{
    if ( val != 0 ) {
        int64_t wrk = val;
        if ( wrk < 0 ) {
            _sign = NEG;
            wrk = -wrk;
        }
        while ( wrk ) {
            mpush_back( wrk % 10 );
            wrk /= 10;
        }
    }
}

// accept a string of digits in canonical form
// e.g. 1234567890
// - leading zeros are ignored (removed)
// - the remaining digits are the significant digit count
// - the number is then loaded into the buffer LSD first
bigly::bigly(std::string num) 
: bigly()
{
    auto itr = num.begin();
    for( ; itr != num.end() && (std::isspace(*itr) || *itr == '0'); ++itr)
        ;
    if ( itr == num.end() ) {
        // value is zero
        // m = 0;
        return;
    }
    // check for leading sign - if first char is '-' or '+' then set s accordingly
    bool signFound = false;
    if (*itr == '-' || *itr == '+') {
        _sign = (sign_t)(*itr++ == '-');
        signFound = true;
    }
    while ( itr != num.end() && std::isdigit(*itr)) {
        mpush_back(*itr++ - '0');
    }
    // check for trailing sign - if next char is '-' or '+' then set s accordingly
    if (itr != num.end() && !signFound && (*itr == '-' || *itr == '+')) {
        _sign = (sign_t)(*itr == '-');
    }
}

bigly::bigly(const int64_t mant, const int64_t frac)
: bigly()
{
}

bigly::~bigly() {
    if ( _m ) 
        delete [] _m;
    _m = nullptr;
    _size = 0;
    _blkcnt = 0;
}

void bigly::resize() {
    _blkcnt++;
    size_t new_z = BLOCK_SIZE * _blkcnt;
    digit_t *q = new digit_t[new_z];
    std::memset(q, 0x00, new_z);
    std::memcpy(q, _m, _size);
    delete [] _m;
    _m = q;
    _size = new_z;
}

void bigly::insert(size_t where, digit_t what) {
    // is there room in the buffer?
    // if not - expand the buffer!
    // TODO: buffer needs to be dynamic memory
    if ( _len == _size ) {
        resize();
    }

    digit_t *p = _m + where;
    std::memcpy( p + 1, p, _size - where - 1 );
    *p = what;
    _len++;
}

int bigly::compare(const bigly& rhs) const {
    // lhs has more signifant digits, or lhs positive and rhs negative
    if ( mant() > rhs.mant() || (mant() > rhs.mant())) return 1;
    // rhs has more signifant digits, or lhs negative and rhs positive
    if ( mant() < rhs.mant() || (mant() < rhs.mant())) return -1;
    // lhs and rhs have same sign and number of signifiant digits
    auto l = begin();
    auto r = rhs.begin();
    while ( l != end() ) {
        if ( *l != *r ) {
            int diff = ( *l - *r ) * sign();
            return diff;
        }
        l++, r++;
    }
    return 0;
}

bigly& bigly::strip_leading(digit_t dig) {
    auto p = begin();
    while ( p > _m && *p == dig )
        _len--;
    return *this;
}

bigly& bigly::operator++() {
    *this += ONE;
    return *this;
}

bigly bigly::operator++(int) {
    auto ret = *this;
    ++(*this);
    return ret;
}

bigly& bigly::operator--() {
    *this -= ONE;
    return *this;
}

bigly bigly::operator--(int) {
    auto ret = *this;
    --(*this);
    return ret;
}

bigly& bigly::operator=(const bigly& o) {
    _sign   = o._sign;
    _len    = o._len;
    _frac   = o._frac;
    _blkcnt = o._blkcnt;
    _size   = o._size;
    std::memcpy(_m, o._m, _size);       
    return *this;
}


bigly bigly::factorial(size_t n) {
    bigly ret = 1;
    bigly cnt = n;
    while( !cnt.is_zero() ) 
        ret = ret * cnt--;
    return ret;
}

bigly bigly::fibonacci(size_t n) {
    if ( n == 0 || n == 1 )
        return n;
    bigly prev = ZERO;
    bigly fib = ONE;
    for ( size_t i = 2; i <= n; ++i ) {
        bigly tmp = fib;
        fib  = prev + fib;
        prev = tmp;
    }
    return fib;
}

} // end namespace cflib