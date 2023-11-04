#include "big_number"

// basic add ability
//
// this does basic columnar arithmetic
// 
big_number big_number::operator+(const big_number& rhs) const {
    if ( rhs.sign() < 0 ) {
        return *this - rhs.negate();
    }
    if (sign() < 0) {
        return rhs - negate();
    }
    return add(rhs);
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
    size_t len = std::max(size(), rhs.size());
    for ( size_t idx(0); idx < len; ++idx ) {
        l = (idx < size()) ? *a++ : 0;
        r = (idx < rhs.size()) ? *b++ : 0;
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
