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

bigly bigly::operator*(const bigly& rhs) const {
    bigly prod;
    digit_t carry(0);
    digit_t digit;
    digit_t l;
    digit_t r;
    auto a = mend();
    auto b = rhs.mend();
    // take a digit from each factor, multiply them together, then
    // prod = d
    for ( size_t r = 0; r < rhs.mant(); ++r ) {
        const digit_t x = *b++;
        if ( x == 0 )
            continue;
        bigly i;
        size_t c = r;
        while( c-- )
            i.mpush_back(0);
        auto s = i.begin();
        std::advance(s, r);
        auto a = begin();
        for ( size_t l = 0; l < mant(); ++l ) {
            digit = (*a++ * x) + carry;
            i.mpush_back(digit % 10);
            carry = digit / 10;
        }
        while (carry) {
            i.mpush_back(carry % 10);
            carry /= 10;
        }
        prod += i;
        std::cout << i << ' ' << prod << std::endl;
    }
    // sign correction - if either factor is negative, but not both
    // then product is negative.
    if ( ( is_negative() || rhs.is_negative() ) && sign() != rhs.sign() ) {
        prod.set_sign(NEG);
    }
    return prod.strip_leading(0);
}

bigly& bigly::operator*=(const bigly&rhs) {
    *this = *this * rhs;
    return *this;
}

bigly bigly::operator^(const bigly& rhs) const {
    bigly prod(*this);
    bigly exp(rhs);
    while ( --exp > ZERO ) {
        prod *= *this;
    }
    return prod;
}

bigly& bigly::operator^=(const bigly&rhs) {
    *this = *this ^ rhs;
    return *this;
}
} // end namespace cflib