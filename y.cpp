#include <iostream>
#include <cstring>
#include <regex>

#include <cassert>

#define SIZE 1024
typedef signed char digit_t;

enum Sign : bool {
    SIGN_POS = false,
    SIGN_NEG = true,
};

struct numb {
    Sign    s;
    digit_t b[SIZE];
    numb();
    numb(const numb& oth);
    numb(const std::string& val);
    numb(const int64_t val);

    size_t width() const;

    numb& nines();
    numb& operator=(const numb& rhs);
    numb operator+(const numb& rhs) const;
    numb& operator+=(const numb& rhs);
    numb operator-(const numb& rhs) const;
    numb operator*(const numb& rhs) const;
    digit_t add(const digit_t d);

    friend std::ostream& operator<<(std::ostream& os, const numb& n);
};

numb::numb()
: s(SIGN_POS)
{
    std::memset(b, 0x00, SIZE);
}

numb::numb(const numb& oth)
: numb()
{
    s = oth.s;
    std::memcpy(b, oth.b, SIZE);
}

numb::numb(const std::string& val) 
:numb()
{
    bool coll_frac = true;
    signed char *p = b;
    // going from LSD to MSD
    for( auto itr = val.rbegin(); itr != val.rend(); ++itr ) {
        signed char c = *itr;
        if ( std::isdigit(c) ) {
            *p++ = c - '0';
        } else if ( c == '+' || c == '-' ) {
            s = (Sign)(c == '-'); // SIGN_NEG
        }
    }
    if ( s )
        nines();
}

numb::numb(const int64_t val) 
: numb()
{
    int64_t v = val;
    if ( v < 0 ) { 
        s = SIGN_NEG;
        v = -v;
    }
    digit_t *p = b;
    while ( v ) {
        *p++ = v % 10;
        v    = v / 10;
    }    
    if ( s )
        nines();
}

size_t numb::width() const {
    // for negative numbers, we ignore leading 9, else leading 0
    digit_t test = (s) ? 9 : 0;
    for( size_t ret = SIZE; ret > 0; --ret )
        if ( b[ret - 1] != test )
            return ret;
    return 0;
}

numb& numb::nines() {
    for( size_t n = 0; n < SIZE; n++)
        b[n] = (digit_t)(9 - b[n]);
    return *this;
}

numb& numb::operator=(const numb& rhs) {
    s = rhs.s;
    std::memcpy(b, rhs.b, SIZE);
    return *this;
}

numb numb::operator+(const numb& rhs) const {
    numb res;
    digit_t carry = 0;
    const digit_t *p = b;
    const digit_t *q = rhs.b;
    digit_t *r = res.b;
    for ( int i = 0; i < SIZE; i++ ) {
        digit_t sum = *p++ + *q++ + carry;
        *r++  = sum % 10;
        carry = sum / 10;
    }
    if ( carry ) {
        if ( s || rhs.s ) {
            // add the carry into the sum
            res.add(carry);
            if ( s && rhs.s ) {
                res.s = SIGN_NEG;
            }
        } else {
            *r = carry;
        }
    } else if ( s || rhs.s ) {
        // no carry, then result is negative and in nines complement
        res.s = SIGN_NEG;
    }
    return res;
}

numb& numb::operator+=(const numb& rhs) {
    *this = *this + rhs;
    return *this;
}

numb numb::operator-(const numb& rhs) const {
    numb neg(rhs);
    neg.nines();
    neg.s = (Sign)(!neg.s);
    return *this + neg;
}

digit_t numb::add(const digit_t d) {
    digit_t carry = d;
    digit_t *r = b;
    for ( int i = 0; i < SIZE; i++ ) {
        digit_t sum = *r + carry;
        *r++  = sum % 10;
        carry = sum / 10;
    }
    return carry;
}

numb numb::operator*(const numb& rhs) const {
    numb res;
    numb ls = *this;
    numb rs = rhs;
    if ( ls.s ) 
        ls.nines();
    if ( rs.s )
        rs.nines();
    size_t lw = ls.width();
    size_t rw = rs.width();
    digit_t carry = 0;
    const digit_t *p = ls.b;
    for ( int i = 0; i < lw; i++ ) {
        numb cum;
        digit_t u = *p++;
        const digit_t *q = rs.b;
        digit_t *c = cum.b + i;
        size_t j = rw;
        while( j-- ) {
            digit_t prod = (u * *q++) + carry;
            *c++  = prod % 10;
            carry = prod / 10;
            assert( c - cum.b < SIZE );
        }
        *c = carry;
        res += cum;
    }
    if ( ls.s ^ rs.s ) {
        res.nines();
        res.s = SIGN_NEG;
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const numb& n) {
    bool nonzero = false;
    int  cnt = 0;
    char sign = 0;
    auto work = n;
    if ( work.s ) {
        sign = '-';
        work.nines();
    }
    size_t wid = n.width();
    digit_t *p = work.b + wid - 1;
    while( wid-- ) {
        digit_t ch = *p--;
        if ((ch == 0 && nonzero) || ch != 0 ) {
            if ( sign ) {
                os << sign;
                sign = 0;
            }
            os << (char)(ch + '0');
            if ( ch != 0 )
                nonzero = true;
            cnt++;
        }
    }
    if ( !cnt )
        os << "0";
    return os;
}

int main() {
    numb a("-238547230598723509837450287235");
    numb b("89734958725098275203985723059822304958724");
    numb c = a + b;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    c = b + a;
    std::cout << c << std::endl;

    c = a - b;
    std::cout << c << std::endl;

    c = b - a;
    std::cout << c << std::endl;

    c = a * b;
    std::cout << c << std::endl;

    return 0;
}

