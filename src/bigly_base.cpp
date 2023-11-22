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
: s(POS)
, d(0)
{
    m.clear();
}

bigly::bigly(const int64_t val) 
: bigly()
{
    if ( val != 0 ) {
        int64_t wrk = val;
        if ( wrk < 0 ) {
            s = NEG;
            wrk = -wrk;
        }
        while ( wrk ) {
            append( wrk % 10 );
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
        s = (sign_t)(*itr++ == '-');
        signFound = true;
    }
    m.clear();
    while ( itr != num.end() && std::isdigit(*itr)) {
        prepend(*itr++ - '0');
    }
    // check for trailing sign - if next char is '-' or '+' then set s accordingly
    if (itr != num.end() && !signFound && (*itr == '-' || *itr == '+')) {
        s = (sign_t)(*itr == '-');
    }
}

bigly::bigly(const int64_t mant, const int64_t frac)
: bigly()
{
}

// iterators
// two contexts - mantiassa and fraction
// mantissa is from advance(begin(), d)..end()
// fraction is begin()..advance(begin(), d-1)
// if d is zero then no fraction.
std::vector<bigly::digit_t>::iterator bigly::begin() {
    auto ret = m.begin();
    if ( d ) {
        std::advance(ret, d);
    }
    return ret;
}

std::vector<bigly::digit_t>::iterator bigly::end() {
    return m.end(); 
}

std::vector<bigly::digit_t>::reverse_iterator bigly::rbegin() {
    auto ret = m.rbegin();
    if ( d ) {
        std::advance(ret, d);
    }
    return ret;
}

std::vector<bigly::digit_t>::reverse_iterator bigly::rend() {
    return m.rend(); 
}

std::vector<bigly::digit_t>::const_iterator bigly::cbegin() const {
    auto ret = m.cbegin();
    if ( d ) {
        std::advance(ret, d);
    }
    return ret;
}

std::vector<bigly::digit_t>::const_iterator bigly::cend() const { 
    return m.cend(); 
}

std::vector<bigly::digit_t>::const_reverse_iterator bigly::crbegin() const { 
    auto ret = m.crbegin();
    if ( d ) {
        std::advance(ret, d);
    }
    return ret;
}

std::vector<bigly::digit_t>::const_reverse_iterator bigly::crend() const { 
    return m.crend(); 
}

// fraction iterators
std::vector<bigly::digit_t>::iterator bigly::fbegin() { 
    if ( !d ) return m.end();
    return m.begin();
}

std::vector<bigly::digit_t>::iterator bigly::fend() { 
    if ( !d ) return m.end();
    auto ret = m.begin();
    std::advance(ret, d - 1);
    return ret; 
}

std::vector<bigly::digit_t>::reverse_iterator bigly::frbegin() { 
    if ( !d ) return m.rend();
    return m.rbegin(); 
}

std::vector<bigly::digit_t>::reverse_iterator bigly::frend() { 
    if ( !d ) return m.rend();
    auto ret = m.rbegin();
    std::advance(ret, d - 1);
    return ret; 
}

std::vector<bigly::digit_t>::const_iterator bigly::fcbegin() const { 
    if ( !d ) return m.cend();
    return m.cbegin(); 
}

std::vector<bigly::digit_t>::const_iterator bigly::fcend() const { 
    if ( !d ) return m.cend();
    auto ret = m.cbegin();
    std::advance(ret, d - 1);
    return ret; 
}

std::vector<bigly::digit_t>::const_reverse_iterator bigly::fcrbegin() const { 
    if ( !d ) return m.crend();
    return m.crbegin(); 
}

std::vector<bigly::digit_t>::const_reverse_iterator bigly::fcrend() const { 
    if ( !d ) return m.crend();
    auto ret = m.crbegin();
    std::advance(ret, d - 1);
    return ret; 
}

int bigly::compare(const bigly& rhs) const {
    // lhs has more signifant digits, or lhs positive and rhs negative
    if ( magn() > rhs.magn() || (magn() > rhs.magn())) return 1;
    // rhs has more signifant digits, or lhs negative and rhs positive
    if ( magn() < rhs.magn() || (magn() < rhs.magn())) return -1;
    // lhs and rhs have same sign and number of signifiant digits
    auto l = crbegin();
    auto r = rhs.crbegin();
    while ( l != crend() ) {
        if ( *l != *r ) {
            int diff = ( *l - *r ) * sign();
            return diff;
        }
        l++, r++;
    }
    return 0;
}

bigly& bigly::strip_leading(digit_t dig) {
    while ( m.begin() != m.end() && dig == m.back() )
        m.pop_back();
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

// append digit to the end of the number (MSD)
void bigly::append(const digit_t digit) {
    m.insert(m.end(), digit);
}

// insert digit to the beginning of the number (LSD)
void bigly::prepend(const digit_t digit) {
    m.insert(m.begin(), digit);
}

// append frac digit to the end of the fraction
void bigly::appfrac(const digit_t digit) {
    m.insert(fend(), digit);
    d++;
}

// insert digit to the beginning of the fraction
void bigly::prefrac(const digit_t digit) {
    m.insert(fbegin(), digit);
    d++;
}

// remove cnt MSD's
void bigly::truncate(size_t cnt) {
    while( magn() && cnt-- ) {
        m.erase(m.end());
    }
}

void bigly::reverse() {
    std::reverse(begin(), end());
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