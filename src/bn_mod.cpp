#include "big_number"


// integer division. Determine the number of times rhs goes into lhs,
// and return the count as big_number. Return any remainder in rem.
big_number big_number::int_division(const big_number& den, big_number& rem) const {
    // this is the numerator (N)
    // if the number of digits in denomenator is more than numerator, 
    // then answer is 0rN
    if ( magn() < den.magn() ) {
        rem = *this;
        return ZERO;
    }

    rem = 0;
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
    big_number quot;
    big_number div;     // divisor
    digit_t cnt;
    const digit_t *p = c_get_msd();
    size_t c = magn();
    while ( c-- ) {
        div.prepend(*p--);
        std::cout << quot << " " << div << " " << rem << std::endl;
        if ( div < den ) {
            quot.prepend(0);
        } else {
            cnt = 0;
            while ( div >= den ) {
                cnt++;
                div -= den;
            }
            quot.prepend(cnt);
            rem = div;
        }
    }
    std::cout << quot << " " << div << " " << rem << std::endl;
    if ( div >= den ) {
        cnt = 0;
        do {
            cnt++;
            div -= den;
        } while ( div >= den );
        quot.prepend(cnt);
        rem = div;
    }

    return quot.strip_leading();
}

// modulus artithmetic
// returns the remainder of lhs / rhs
big_number big_number::operator%(const big_number& rhs) const {
    big_number rem;
    int_division(rhs, rem);
    return rem;
}

// modulus artithmetic
// returns the remainder of lhs / rhs
big_number& big_number::operator%=(const big_number& rhs) {
    big_number rem;
    int_division(rhs, rem);
    *this = rem;
    return *this;
}

// integer division
// returns the count of times rhs fits into lhs.
big_number big_number::operator/(const big_number& rhs) const {
    big_number rem;
    return int_division(rhs, rem);
}

// integer division
// returns the count of times rhs fits into lhs.
big_number& big_number::operator/=(const big_number& rhs) {
    big_number rem;
    *this = int_division(rhs, rem);
    return *this;
}