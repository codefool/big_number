#include <iostream>
#include <cstring>
#include <memory>

template <typename T> int sign_of(T val) {
    return (T(0) < val) - (val < T(0));
}

typedef signed char digit;

const digit FILL_POS = 0;
const digit FILL_NEG = 9;
#define fill(sign) ((sign)?FILL_NEG:FILL_POS)

const bool  SIGN_POS = false;
const bool  SIGN_NEG = true;

#define TO_CHAR(b) (char)(b + '0')
#define FROM_CHAR(c) (digit)(c - '0')

typedef std::shared_ptr<digit[]> digit_buff;

struct buff {
    digit_buff b;   // buffer of digits 
    size_t     bs;  // size of b

    buff() : bs(0) {}

    buff(size_t sz, digit fill, const buff *oth = nullptr)
    : bs(sz)
    {
        init(sz, fill, oth);
    }

    buff& init(size_t sz, digit fill, const buff *oth = nullptr) {
        bs = sz;
        b = std::make_shared<digit[]>(bs);
        std::memset(b.get(), fill, bs);
        if( oth != nullptr ) {
            size_t z = std::min(oth->size(), bs);
            std::memcpy(b.get(), oth->c_get(), z);
        }
        return *this;
    }
    inline digit* get() const { return b.get(); }
    inline const digit *c_get() const { return b.get();}
    inline size_t size() const { return bs; }
};

struct numb {
    buff n; // buffer of digits 
    bool s; // sign - true negative, false positive

    numb() {}
    numb(size_t sz, bool sign = SIGN_POS)
    : s(sign)
    , n(sz, fill(sign))
    {}
    numb(const numb& oth, size_t sz = 0) 
    : s(oth.s)
    , n((!sz)?oth.size():sz, fill(oth.s), &oth.n)
    {}

    numb(const std::string& val);
    buff& get_buff() { return n; }

    digit add(const digit d) {
        digit carry = d;
        digit *r = n.get();
        for( size_t i(0); i < n.size(); ++i ) {
            digit sum = *r + carry;
            *r++ = sum % 10;
            carry = sum / 10;
        }
        return carry;
    }

    numb& nines() {
        digit *p = n.get();
        for (size_t c(n.size()); c > 0; --c, ++p)
            *p = 9 - *p;
        return *this;
    }

    bool abs() {
        bool org_s = s;
        if( s ) {
            nines();
            s = SIGN_POS;
        }
        return org_s;
    }

    inline digit *get() { return n.get(); }
    inline size_t size() const {return n.size();}
    friend std::ostream& operator<<(std::ostream& os, const numb& n);
};

typedef std::unique_ptr<numb> numb_ptr;

numb::numb(const std::string& val) 
: s(false)
{
    bool coll_frac = true;
    buff q(val.length(), FILL_POS);
    digit *p = q.get();
    size_t cnt(0);
    // going from LSD to MSD
    for( auto itr = val.rbegin(); itr != val.rend(); ++itr ) {
        signed char c = *itr;
        if ( std::isdigit(c) ) {
            *p++ = FROM_CHAR(c);
            cnt++;
        } else if ( c == '+' || c == '-' ) {
            s = (c == '-');
        } else if ( c == '.' ) {
            ;
        }
    }
    // now setup our local buffer
    n.init(cnt, fill(s), &q);
    if ( s )
        nines();
}


std::ostream& operator<<(std::ostream& os, const numb& n) {
    bool   nonzero = false;
    size_t cnt = 0;
    auto   work = n;
    const char  *sign = nullptr;
    if ( work.s ) {
        sign = "-";
        work.nines();
    }
    size_t wid = work.size();
    digit *p = work.get() + wid - 1;
    while( wid-- ) {
        digit ch = *p--;
        if ((ch == 0 && nonzero) || ch != 0 ) {
            if ( sign ) {
                os << sign;
                sign = nullptr;
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

struct tuple {
    numb_ptr lhs;
    numb_ptr rhs;

    tuple(const numb& l, const numb& r)
    {
        // align the two values
        size_t sz = std::max(l.size(), r.size());
        lhs = std::make_unique<numb>(l, sz);
        rhs = std::make_unique<numb>(r, sz);
    }

    numb add() {
        // for add/subtract operations, the result will be (at most)
        // n+1 digits long, where n is the common length of lhs/rhs.
        numb& pl = *(lhs.get());
        numb& pr = *(rhs.get());
        size_t sz = pl.size();
        numb result(pl.size() + 1);
        digit carry = 0;
        const digit *p = pl.get();
        const digit *q = pr.get();
        digit *r = result.get();
        for( size_t i = 0; i < sz; i++ ) {
            digit sum = *p++ + *q++ + carry;
            *r++  = sum % 10;
            carry = sum / 10;
        }
        if( carry ) {
            if( pl.s || pr.s ) {
                // add the carry into the sum
                result.add(carry);
                if ( pl.s && pr.s ) {
                    result.s = SIGN_NEG;
                }
            } else {
                *r = carry;
            }
        } else if( pl.s || pr.s ) {
            // no carry, then result is negative and in nines complement
            result.s = SIGN_NEG;
        }
        return result;
    }

    numb mult() {
        // for multiply operations, the result will be (at most)
        // lhs.size() + rhs.size() digits wide
        numb& pl = *(lhs.get());
        numb& pr = *(rhs.get());
        bool  lsgn = pl.abs();
        bool  rsgn = pr.abs();
        numb result(pl.size() + pr.size());
        size_t sz = pl.size();
        const digit *p = pl.get();
        // for every digit in lhs
        for( size_t lcnt = 0; lcnt < sz; ++lcnt ) {
            digit carry = 0;
            digit d = *p++;
            numb accum(pl.size() + pr.size());
            // shift result left lcnt digits
            digit *acc = accum.get() + lcnt;
            // multiply by each digit of rhs
            const digit *q = pr.get();
            for( size_t i = 0; i < sz; i++ ) {
                digit prod = (d * *q++) + carry;
                *acc++ = prod % 10;
                carry  = prod / 10;
            }
            if( carry ) {
                *acc = carry;
            }
            result = tuple(result, accum).add();
        }
        if( lsgn != rsgn ) {
            result.nines();
            result.s = SIGN_NEG;
        }
        return result;
    }
};

int main() {
    numb a("-238547230598723509837450287235");
    std::cout << a << std::endl;
    numb b("89734958725098275203985723059822304958724");
    std::cout << b << std::endl;
    tuple t(a,b);
    numb c = t.mult();
    std::cout << c << std::endl;
    return 0;
}
