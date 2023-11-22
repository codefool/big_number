/*
 * bigly - library for arbitrarily large numbers
 *
 * Copyright (C) 2023 Garyl Hester. All rights reserved.
 * 
 * This project lives at https://github.com/codefool/bigly
 *
 */
#include "bigly"

namespace cflib {
// basic add ability
//
// this does basic columnar arithmetic
// 
bigly bigly::operator+(const bigly& rhs) const {
    // a + -b === a - b
    if ( rhs.is_negative() ) {
        return *this - rhs.abs();
    }
    if ( is_negative() ) {
        return rhs - abs();
    }
    return add(rhs);
}

bigly& bigly::operator+=(const bigly& rhs) {
    *this = *this + rhs;
    return *this;
}

bigly bigly::add(const bigly& rhs) const {
    bigly sum;
    digit_t carry(0);
    digit_t digit;
    digit_t l;
    digit_t r;
    auto s = sum.mbegin();
    auto a = mbegin();
    auto b = rhs.mbegin();
    // take a digit from each addend, add then together, then
    // sum = d
    size_t len = std::max(mant(), rhs.mant());
    for ( size_t idx(0); idx < len; ++idx ) {
        l = (idx < mant())     ? *a++ : 0;
        r = (idx < rhs.mant()) ? *b++ : 0;
        digit = l + r + carry;
        sum.mpush_front(digit % 10);
        carry = digit / 10;
    }
    if (carry) {
        sum.mpush_front(carry);
    }
    return sum;
}
} // end namespace cflib
