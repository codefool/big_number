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
// subtract two positive numbers using complement method
bigly bigly::operator-(const bigly& rhs) const {
    if ( *this == rhs ) {
        return bigly::ZERO;
    }
    if ( is_negative() ) { 
        if ( rhs.is_positive() ) {
            // -a - b === -(a + b)
            return ( negate() + rhs ).negate();
        }
        // -a - -b === -a + b === b - a
        return ( negate() - rhs.negate() ).negate();
    }
    if ( mant() != rhs.mant() ) {
        bigly res;
        if ( mant() > rhs.mant() ) {
            res = sub_method_diff_magn(rhs).strip_leading();
        } else {
            res = rhs.sub_method_diff_magn(*this).strip_leading();
        }
        // sign magic - if magn() < rhs.magn()
        // then the sign of the result is inverse of lhs
        if ( mant() < rhs.mant() ) {
            res.s = (sign_t)is_positive();
        }
        return res;
    }

    return sub_method_one(rhs).strip_leading();
}

bigly& bigly::operator-=(const bigly& rhs) {
    *this = *this - rhs;
    return *this;
}

// calculate and return the nines complement
// The nines' complement of a decimal digit is the number that must
// be added to it to produce 9, e.g. the nines' complement of 3 is 6.
//
bigly bigly::nines_complement(const size_t len) const {
    bigly cmp(*this);  
    int idx(0);  
    for ( ; idx < mant(); ++idx) {
        cmp[idx] = 9 - cmp[idx];
    }
    for ( ; idx < len; ++idx) {
        cmp.mpush_back(9 - cmp[idx]);
    }
    return cmp;
}

// 1. Compute the nines' complement of the minuend
// 2. Add it to the subtrahend
// 3. Compute the nines' complement of the result
bigly bigly::sub_method_one(const bigly& rhs) const {
    bigly cmp = nines_complement(rhs.mant());
    bigly res = cmp + rhs;
    return res.nines_complement(res.mant());
}

// 1. Compute the nines' complement of the subtrahend
// 2. Add it to the minuend
// 3. Drop the leading '1'
// 4. Add 1 to the answer.
bigly bigly::sub_method_two(const bigly& rhs) const {
    bigly cmp = rhs.nines_complement(rhs.mant());
    bigly res = *this + cmp;
    res.mpop_front();
    // res.b[--res.m] = 0;
    res += bigly::ONE;
    return res;
}

// if the subtrahend has fewer digits than the minuend, leading zeros must be
// added in the second method. The zeros become the leading nines when 
// complement is taken.
bigly bigly::sub_method_diff_magn(const bigly& rhs) const {
    bigly inter(rhs);
    while(inter.mant() < mant())
        inter.mpush_back(0);
    auto res = sub_method_two(inter);
    return res;
}
} // end namespace cflib