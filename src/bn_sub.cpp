#include "big_number"

// subtract two positive numbers using complement method
big_number big_number::operator-(const big_number& rhs) const {
    if ( *this == rhs ) {
        return big_number::ZERO;
    }
    if ( is_negative() ) { 
        if ( rhs.is_positive() ) {
            // -a - b === -(a + b)
            return ( negate() + rhs ).negate();
        }
        // -a - -b === -a + b === b - a
        return ( negate() - rhs.negate() ).negate();
    }
    if ( magn() != rhs.magn() ) {
        big_number res;
        if ( magn() > rhs.magn() ) {
            res = sub_method_diff_magn(rhs).strip_leading();
        } else {
            res = rhs.sub_method_diff_magn(*this).strip_leading();
        }
        // sign magic - if magn() < rhs.magn()
        // then the sign of the result is inverse of lhs
        if ( magn() < rhs.magn() ) {
            res.s = (sign_t)is_positive();
        }
        return res;
    }

    return sub_method_one(rhs).strip_leading();
}

big_number& big_number::operator-=(const big_number& rhs) {
    *this = *this - rhs;
    return *this;
}

// calculate and return the nines complement
// The nines' complement of a decimal digit is the number that must
// be added to it to produce 9, e.g. the nines' complement of 3 is 6.
//
big_number big_number::nines_complement(const size_t len) const {
    big_number cmp(*this);  
    int idx(0);  
    for ( ; idx < magn(); ++idx) {
        cmp[idx] = 9 - cmp[idx];
    }
    for ( ; idx < len; ++idx) {
        cmp.append(9 - cmp[idx]);
    }
    return cmp;
}

// 1. Compute the nines' complement of the minuend
// 2. Add it to the subtrahend
// 3. Compute the nines' complement of the result
big_number big_number::sub_method_one(const big_number& rhs) const {
    big_number cmp = nines_complement(rhs.magn());
    big_number res = cmp + rhs;
    return res.nines_complement(res.magn());
}

// 1. Compute the nines' complement of the subtrahend
// 2. Add it to the minuend
// 3. Drop the leading '1'
// 4. Add 1 to the answer.
big_number big_number::sub_method_two(const big_number& rhs) const {
    big_number cmp = rhs.nines_complement(rhs.magn());
    big_number res = *this + cmp;
    res.truncate(1);
    // res.b[--res.m] = 0;
    res += big_number::ONE;
    return res;
}

// if the subtrahend has fewer digits than the minuend, leading zeros must be
// added in the second method. The zeros become the leading nines when 
// complement is taken.
big_number big_number::sub_method_diff_magn(const big_number& rhs) const {
    big_number inter(rhs);
    while(inter.magn() < magn())
        inter.append(0);
    auto res = sub_method_two(inter);
    return res;
}