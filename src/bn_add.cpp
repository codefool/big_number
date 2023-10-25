#include "big_number"

// basic add ability
//
// this does basic columnar arithmetic
// 
big_number big_number::operator+(const big_number& rhs) const {
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
    size_t len = (size() > rhs.size()) ? size() : rhs.size();
    for ( size_t idx(0); idx < len; ++idx ) {
        l = (idx < size()) ? *a++ : 0;
        r = (idx < rhs.size()) ? *b++ : 0;
        digit = l + r + carry;
        *s++ = digit % 10;
        sum.d++;
        carry = digit / 10;
    }
    if (carry) {
        *s++ = carry;
        sum.d++;
    }
    return sum;
}

// calculate and return the nines complement
// The nines' complement of a decimal digit is the number that must
// be added to it to produce 9, the nines' complement of 3 is 6.
//
big_number big_number::nines_complement() const {
    big_number cmp(*this);    
    for ( int idx = 0; idx < B_SIZE; ++idx) {
        cmp[idx] = 9 - cmp[idx];
    }
    return cmp;
}

// subtract two positive numbers using complement method
big_number big_number::operator-(const big_number& rhs) const {
    big_number cmp = nines_complement();
    big_number res = cmp + rhs;
    return res.nines_complement();
}