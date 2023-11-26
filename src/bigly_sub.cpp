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
    const bigly& lhs = *this;
    if ( lhs == rhs ) {
        return bigly::ZERO;
    }
    if ( lhs.is_negative() ) { 
        if ( rhs.is_positive() ) {
            // -a - b === -(a + b)
            return ( lhs.negate() + rhs ).negate();
        }
        // -a - -b === -a + b === b - a
        return ( lhs.negate() - rhs.negate() ).negate();
    }
    // lhs is positive
    if ( rhs.is_negative() ) {
        // a - -b = a + b
        return lhs + rhs.negate();
    } else if( lhs < rhs ) {
        // if a < b, then a - b = -(b-a)
        return ( rhs - *this ).negate();
    }
    if ( mant() != rhs.mant() ) {
        bigly res = ( mant() > rhs.mant() )
                  ? lhs.sub_method_diff_magn(rhs).strip_leading()
                  : rhs.sub_method_diff_magn(lhs).strip_leading();

        // sign magic - if magn() < rhs.magn() then res is negative
        if ( mant() < rhs.mant() ) {
            res.set_sign(NEG);
        }
        return res;
    }

    return lhs.sub_method_one(rhs).strip_leading();
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
    std::cout << "b9c " << cmp << std::endl;
    int idx(0);  
    for ( ; idx < mant(); ++idx) {
        cmp[idx] = 9 - cmp[idx];
    }
    for ( ; idx < len; ++idx) {
        cmp.mpush_back(9);
    }
    std::cout << "a9c " << cmp << std::endl;
    return cmp;
}

// 1. Compute the nines' complement of the minuend
// 2. Add it to the subtrahend
// 3. Compute the nines' complement of the result
bigly bigly::sub_method_one(const bigly& rhs) const {
    bigly cmp = nines_complement(rhs.mant());
    std::cout << *this << ' ' << cmp << ' ' << rhs << std::endl;
    bigly res = cmp + rhs;
    std::cout << cmp << ' ' << rhs << std::endl;
    cmp = res.nines_complement(res.mant());
    std::cout << *this << ' ' << cmp << std::endl;
    return cmp;
}

// 1. Compute the nines' complement of the subtrahend
// 2. Add it to the minuend
// 3. Drop the leading '1'
// 4. Add 1 to the answer.
// 5. Strip leading zeros
bigly bigly::sub_method_two(const bigly& rhs) const {
    bigly cmp = rhs.nines_complement(rhs.mant());
    std::cout << *this << ' ' << cmp << std::endl;
    bigly res = *this + cmp;
    std::cout << "a " << res << std::endl;
    res.mpop_back();
    std::cout << "b " << res << std::endl;
    res += bigly::ONE;
    std::cout << "c " << res << std::endl;
    res.strip_leading(0);
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