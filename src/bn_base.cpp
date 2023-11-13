/*
 * big_number - library for arbitrarily large numbers
 *
 * Copyright (C) 2023 Garyl Hester. All rights reserved.
 * 
 * This project lives at https://github.com/codefool/big_number
 *
 */
#include "big_number"
#include <algorithm>

namespace cflib {
big_number big_number::ONE("1");
big_number big_number::ZERO("0");

big_number::big_number() 
: s(POS)
{
    b.clear();
}

big_number::big_number(const int64_t val) {
    if ( val == 0 ) {
        s = POS;
        return;
    } 
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

// accept a string of digits in canonical form
// e.g. 1234567890
// - leading zeros are ignored (removed)
// - the remaining digits are the significant digit count
// - the number is then loaded into the buffer LSD first
big_number::big_number(std::string num) 
: big_number()
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
    b.clear();
    while ( itr != num.end() && std::isdigit(*itr)) {
        prepend(*itr++ - '0');
    }
    // b.erase(b.end());
    // check for trailing sign - if next char is '-' or '+' then set s accordingly
    if (itr != num.end() && !signFound && (*itr == '-' || *itr == '+')) {
        s = (sign_t)(*itr == '-');
    }
}

int big_number::compare(const big_number& rhs) const {
    // lhs has more signifant digits, or lhs positive and rhs negative
    if ( magn() > rhs.magn() || (magn() > rhs.magn())) return 1;
    // rhs has more signifant digits, or lhs negative and rhs positive
    if ( magn() < rhs.magn() || (magn() < rhs.magn())) return -1;
    // lhs and rhs have same sign and number of signifiant digits
    auto l = b.crbegin();
    auto r = rhs.b.crbegin();
    while ( l != b.crend() ) {
        if ( *l != *r ) {
            int diff = ( *l - *r ) * sign();
            return diff;
        }
        l++, r++;
    }
    return 0;
}

big_number& big_number::strip_leading(digit_t dig) {
    while ( b.begin() != b.end() && dig == b.back() )
        b.pop_back();
    return *this;
}

big_number& big_number::operator++() {
    *this += ONE;
    return *this;
}

big_number big_number::operator++(int) {
    auto ret = *this;
    ++(*this);
    return ret;
}

big_number& big_number::operator--() {
    *this -= ONE;
    return *this;
}

big_number big_number::operator--(int) {
    auto ret = *this;
    --(*this);
    return ret;
}

// append digit to the end of the number (MSD)
void big_number::append(const digit_t digit) {
    b.push_back(digit);
}

// insert digit to the beginning of the number (LSD)
void big_number::prepend(const digit_t digit) {
    b.insert(b.begin(), digit);
}

// remove cnt MSD's
void big_number::truncate(size_t cnt) {
    while( magn() && cnt-- ) {
        b.erase(b.end());
    }
}

void big_number::reverse() {
    std::reverse(begin(), end());
}

big_number big_number::factorial(size_t n) {
    big_number ret = 1;
    big_number cnt = n;
    while( !cnt.is_zero() ) 
        ret = ret * cnt--;
    return ret;
}

big_number big_number::fibonacci(size_t n) {
    if ( n == 0 || n == 1 )
        return n;
    big_number prev = ZERO;
    big_number fib = ONE;
    for ( size_t i = 2; i <= n; ++i ) {
        big_number tmp = fib;
        fib  = prev + fib;
        prev = tmp;
    }
    return fib;
}
} // end namespace cflib