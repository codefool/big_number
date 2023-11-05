#include "big_number"

big_number big_number::ONE("1");
big_number big_number::ZERO("0");

big_number::big_number() 
: s(POS), 
  m(0),
  b(std::make_shared<buff_t>())
{
    std::memset(get(), 0x00, B_SIZE);
}

big_number::big_number(const big_number& other)
: big_number()
{
    std::memcpy((void *)get(), (const void *)other.c_get(), other.magn());
    m = other.m;
    s = other.s;
}

// accept a string of digits in canonical form
// e.g. 1234567890
// - leading zeros are ignored (removed)
// - the remaining digits are the significant digit count
// - the number is then loaded into the buffer LSD first
big_number::big_number(std::string num) 
: big_number()
{
    auto itr = num.begin();
    for( ; itr != num.end() && (std::isspace(*itr) || *itr == '0'); ++itr)
        ;
    if ( itr == num.end() ) {
        // value is zero
        get()[0] == 0;
        m++;
        return;
    }
    // check for leading sign - if first char is '-' or '+' then set s accordingly
    bool signFound = false;
    if (*itr == '-' || *itr == '+') {
        s = (sign_t)(*itr++ == '-');
        signFound = true;
    }
    auto p = get();
    while ( itr != num.end() && std::isdigit(*itr)) {
        *p++ = *itr++ - '0';
        m++;
    }
    // check for trailing sign - if next char is '-' or '+' then set s accordingly
    if (itr != num.end() && !signFound && (*itr == '-' || *itr == '+')) {
        s = (sign_t)(*itr == '-');
    }
    auto q = get();
    while( p-- > q ) {
        auto t = *p;
        *p   = *q;
        *q++ = t;
    }
}

int big_number::compare(const big_number& rhs) const {
    // lhs has more signifant digits, or lhs positive and rhs negative
    if ( magn() > rhs.magn() || (magn() > rhs.magn())) return 1;
    // rhs has more signifant digits, or lhs negative and rhs positive
    if ( magn() < rhs.magn() || (magn() < rhs.magn())) return -1;
    // lhs and rhs have same sign and number of signifiant digits
    const digit_t *l = c_get() + magn() - 1;
    const digit_t *r = rhs.c_get() + magn() - 1;
    for( auto digit = magn(); digit; l--, r--, digit-- ) {
        if ( *l != *r ) {
            int diff = (*l - *r) * sign();
            return diff;
        }
    }
    return 0;
}

big_number& big_number::strip_leading(digit_t dig) {
    auto front = get();
    auto end   = front + magn() - 1;
    while ( end > front && dig == *end-- ) {
        m--;
    }
    return *this;
}
