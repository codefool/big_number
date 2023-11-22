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

// integer division. Determine the number of times rhs goes into lhs,
// and return the count as bigly. Return any remainder in rem.
bigly bigly::int_division(const bigly& den, bigly& rem) const {
    // this is the numerator (N)
    // if the number of digits in denomenator is more than numerator, 
    // then answer is 0rN
    if ( mant() < den.mant() ) {
        rem = *this;
        return ZERO;
    }

    // long divison.
    //       00100371 r 45 => 100371 r 45
    // 123 | 12345678
    //       123
    //          456
    //          369
    //           877
    //           861
    //            168
    //            123
    //             45  
    rem = 0;            // remainder
    bigly quot;    // quotient
    bigly div;     // divisor
    digit_t cnt;
    // const digit_t *p = c_get_msd();
    auto p = mend();
    while ( p != mbegin() ) {
        div.mpush_back(*p--);
        if ( div < den ) {
            quot.mpush_back(0);
        } else {
            cnt = 0;
            while ( div >= den ) {
                cnt++;
                div -= den;
            }
            quot.mpush_back(cnt);
            rem = div;
        }
    }
    if ( div >= den ) {
        cnt = 0;
        do {
            cnt++;
            div -= den;
        } while ( div >= den );
        quot.mpush_back(cnt);
        rem = div;
    }

    return quot.strip_leading();
}

// modulus artithmetic
// returns the remainder of lhs / rhs
bigly bigly::operator%(const bigly& rhs) const {
    bigly rem;
    int_division(rhs, rem);
    return rem;
}

// modulus artithmetic
// returns the remainder of lhs / rhs
bigly& bigly::operator%=(const bigly& rhs) {
    bigly rem;
    int_division(rhs, rem);
    *this = rem;
    return *this;
}

// integer division
// returns the count of times rhs fits into lhs.
bigly bigly::operator/(const bigly& rhs) const {
    bigly rem;
    return int_division(rhs, rem);
}

// integer division
// returns the count of times rhs fits into lhs.
bigly& bigly::operator/=(const bigly& rhs) {
    bigly rem;
    *this = int_division(rhs, rem);
    return *this;
}
} // end namespace cflib