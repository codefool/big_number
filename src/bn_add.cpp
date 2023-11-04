#include "big_number"

// basic add ability
//
// this does basic columnar arithmetic
// 
big_number big_number::operator+(const big_number& rhs) const {
    // a + -b === a - b
    if ( rhs.is_negative() ) {
        return *this - rhs.abs();
    }
    if ( is_negative() ) {
        return rhs - abs();
    }
    return add(rhs);
}

big_number& big_number::operator+=(const big_number& rhs) {
    *this = *this + rhs;
    return *this;
}

big_number big_number::add(const big_number& rhs) const {
    big_number sum;
    digit_t carry(0);
    digit_t digit;
    digit_t l;
    digit_t r;
    digit_t       *s = sum.get();
    const digit_t *a = c_get();
    const digit_t *b = rhs.c_get();
    // take a digit from each addend, add then together, then
    // sum = d
    size_t len = std::max(magn(), rhs.magn());
    for ( size_t idx(0); idx < len; ++idx ) {
        l = (idx < magn()) ? *a++ : 0;
        r = (idx < rhs.magn()) ? *b++ : 0;
        digit = l + r + carry;
        *s++ = digit % 10;
        sum.m++;
        carry = digit / 10;
    }
    if (carry) {
        *s++ = carry;
        sum.m++;
    }
    return sum;
}

void big_number::append(digit_t digit) {
    if ( m < B_SIZE ) {
        b[m++] = digit;
    }
}
