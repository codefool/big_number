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

#define TO_CHAR(b) (char)(b + '0')
#define FROM_CHAR(c) (digit_t)(c - '0')

struct numb {
    Sign     s;
    size_t   sz;
    digit_t *b;
    numb(size_t size = SIZE, Sign s = SIGN_POS);
    numb(const numb& oth);
    numb(const std::string& val);
    numb(const int64_t val);
    ~numb();

    size_t width() const;

    void resize(size_t new_size);

    numb& nines();
    numb& operator=(const numb& rhs);
    numb operator+(const numb& rhs) const;
    numb& operator+=(const numb& rhs);
    numb operator-(const numb& rhs) const;
    numb operator*(const numb& rhs) const;
    digit_t add(const digit_t d);

    digit_t fill() const {return (s) ? '9' : '0';}
    friend std::ostream& operator<<(std::ostream& os, const numb& n);
};

numb::numb(size_t size, Sign s)
: s(s), sz(size)
{
    b = new digit_t[sz]{};
    std::memset(b, fill(), sz);
}

numb::numb(const numb& oth)
: numb(oth.sz, oth.s)
{
    std::memcpy(b, oth.b, oth.sz);
}

numb::numb(const std::string& val) 
:numb(val.length())
{
    bool coll_frac = true;
    signed char *p = b;
    // going from LSD to MSD
    for( auto itr = val.rbegin(); itr != val.rend(); ++itr ) {
        signed char c = *itr;
        if ( std::isdigit(c) ) {
            *p++ = FROM_CHAR(c);
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

numb::~numb() {
    if ( b != nullptr )
        delete [] b;
    b = nullptr;
    sz = 0;
}

size_t numb::width() const {
    // for negative numbers, we ignore leading 9, else leading 0
    digit_t test = (s) ? 9 : 0;
    for( size_t ret = sz; ret > 0; --ret )
        if ( b[ret - 1] != test )
            return ret;
    return 0;
}

void numb::resize(size_t new_size) {
    if ( new_size > sz ) {
        digit_t *p = new digit_t[new_size]{};
        std::memset(p, fill(), new_size);
        std::memcpy(p, b, sz);
        delete [] b;
        b = p;
        sz = new_size;
    }
}

numb& numb::nines() {
    for( size_t n = 0; n < sz; n++)
        b[n] = (digit_t)(9 - b[n]);
    return *this;
}

numb& numb::operator=(const numb& rhs) {
    s = rhs.s;
    resize(rhs.sz);
    std::memcpy(b, rhs.b, rhs.sz);
    return *this;
}

numb numb::operator+(const numb& rhs) const {
    numb res(std::max(width(),rhs.width()) + 1);
    digit_t carry = 0;
    const digit_t *p = b;
    const digit_t *q = rhs.b;
    digit_t *r = res.b;
    for ( int i = 0; i < sz; i++ ) {
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
    for ( int i = 0; i < sz; i++ ) {
        digit_t sum = *r + carry;
        *r++  = sum % 10;
        carry = sum / 10;
    }
    return carry;
}

numb numb::operator*(const numb& rhs) const {
    size_t prod_sz = sz + rhs.sz;
    numb res(prod_sz);
    numb ls = *this;
    numb rs = rhs;
    Sign lsgn = ls.s;
    Sign rsgn = rs.s;
    if ( ls.s ) {
        ls.nines();
        ls.s = SIGN_POS;
    }
    if ( rs.s ) {
        rs.nines();
        rs.s = SIGN_POS;
    }
    size_t lw = ls.width();
    size_t rw = rs.width();
    digit_t carry = 0;
    const digit_t *p = ls.b;
    for ( int i = 0; i < lw; i++ ) {
        numb cum(prod_sz);
        digit_t u = *p++;
        if ( u == 0 ) 
            continue;
        const digit_t *q = rs.b;
        digit_t *c = cum.b + i;
        size_t j = rw;
        while( j-- ) {
            digit_t prod = (u * *q++) + carry;
            *c++  = prod % 10;
            carry = prod / 10;
            assert( 0 <= prod && prod <= 99 );
            assert( c - cum.b < cum.sz );
        }
        *c = carry;
        res += cum;
        std::cout << TO_CHAR(u) << " " << cum << " " << res.sz << std::endl;
        std::cout << "= " << res << std::endl;
    }
    std::cout << res << std::endl;
    if ( lsgn ^ rsgn ) {
        res.nines();
        res.s = SIGN_NEG;
        std::cout << res << std::endl;
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
            os << TO_CHAR(ch);
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

    std::cout << "a     " << a << std::endl;
    std::cout << "b     " << b << std::endl;
    std::cout << "a + b " << c << std::endl;

    c = b + a;
    std::cout << "b + a " << c << std::endl;

    c = a - b;
    std::cout << "a - b " << c << std::endl;

    c = b - a;
    std::cout << "b - a " << c << std::endl;

    c = a * b;
    std::cout << "a * b " << c << std::endl;

    return 0;
}

