#include "big_number"

big_number big_number::operator*(const big_number& rhs) const {
    big_number prod;
    digit_t carry(0);
    digit_t digit;
    digit_t l;
    digit_t r;
    const digit_t *a = c_get();
    const digit_t *b = rhs.c_get();
    // take a digit from each factor, multiply them together, then
    // prod = d
    for ( size_t r = 0; r < rhs.magn(); ++r ) {
        const digit_t x = *b++;
        if ( x == 0 )
            continue;
        big_number i;
        i.m = r+1;
        digit_t *s = i.get() + r;
        const digit_t *a = c_get();
        for ( size_t l = 0; l < magn(); ++l ) {
            digit = (*a++ * x) + carry;
            *s++ = digit % 10;
            i.m++;
            carry = digit / 10;
        }
        while (carry) {
            *s++ = carry % 10;
            carry /= 10;
            i.m++;
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

big_number& big_number::operator*=(const big_number&rhs) {
    *this = *this * rhs;
    return *this;
}

big_number big_number::operator^(const big_number& rhs) const {
    big_number prod(*this);
    big_number exp(rhs);
    while ( --exp > ZERO ) {
        prod *= *this;
    }
    return prod;
}

big_number& big_number::operator^=(const big_number&rhs) {
    *this = *this ^ rhs;
    return *this;
}


