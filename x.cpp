//    1234.567      1234.567
//  -12345.670     87654.329
//                 88888.896 => -11111.103


#include <iostream>
#include <cstring>
#include <regex>

#define SIZE 100

template <typename T> int sgn_of(T val) {
    return (T(0) < val) - (val < T(0));
}

struct tuple;

struct buff {
    signed char b[SIZE];
    bool   sign; // true positive, false negative
    size_t dec;  // decimal (insertion) point
    size_t size; // size of buffer

    buff(): dec(0), size(0), sign(true) {}

    // buff( size_t sz, size_t dc = 0, bool sgn = true, const buff* p = nullptr )
    // : dec(dc), size(sz), sign(sgn)
    // {
    //     if ( p != nullptr ) {
    //         reset( sz, dc, p );
    //     }
    // }

    // buff( const signed char c )
    // : buff()
    // {
    //     b[0] = c;
    //     size = 1;
    // }

    buff(const std::string val)
    : buff()
    {
        // regex here to make sure this is a signed-number in proper format
        // <digit>  ::= 0|1|2|...|base-1
        // <sign>   ::= +|-
        // <digits> ::= <digit>|<digits><digit>
        // <number> ::= <digits>["."<digits>]
        // <signed-number> ::= <sign><number>|<number><sign>
        // std::smatch m;
        // const std::regex base_regex("^(([+-]?)([0-9]+)(\\.[0-9]+))|(([0-9]+)((\\.[0-9]+)?)([+-]?))$");
        // if ( std::regex_match(val, m, base_regex) ) {
        //     for (auto i=0; i < m.size(); ++i ) {
        //         std::cout << "match" << i << ": " << m[i]
        //                 << " (with a length of " << m[i].length() << ")"
        //                 << std::endl;
                
        //     }
        // }

        bool coll_frac = true;
        signed char *p = b;
        // going from LSD to MSD
        for( auto itr = val.rbegin(); itr != val.rend(); ++itr ) {
            signed char c = *itr;
            if ( std::isdigit(c) ) {
                *p++ = c - '0';
                ++size;
                if ( coll_frac )
                    dec++;
            } else if ( c == '.' ) {
                coll_frac = false;
            } else if ( c == '+' || c == '-' ) {
                sign = (c == '+');
            }
        }
        // at this point, if coll_frac it true, this means we
        // never saw a decimal point, so the number is an
        // integer. Do fixup.
        if ( coll_frac ) {
            dec = 0;
        }
        trim();
    }

    buff(const int64_t val)
    : buff()
    {
        int64_t v = val;
        sign = v >= 0;
        size = dec = 0;
        signed char *p = b;
        while( v > 0 ) {
            *p++ = v % 10;
            v /= 10;
            size++;
        }
    }

    buff& reset( size_t sz, size_t dc = 0, const buff* org = nullptr ) {
        std::memset(b, 0x00, SIZE);
        size = sz;
        dec = dc;
        sign = true;
        // now copy the mant and frac from org into the buffer
        // aligned with the desired dec point.
        // 0000000000.0000000000
        //       1234.4321
        if ( org != nullptr ) {
            sign = org->sign;
            signed char *p = b + (dec - org->dec);
            std::memcpy(p, org->b, org->size);
        }
        return *this;
    }

    // trim leading and trailing zeros from number
    void trim() {
        // in b, find the first non-zero and last non-zero
        // and alter object stats.
        // 0004321.1234000
        // b       d     s
        // p       q     r
        if ( !size )
            return;
        signed char *p = b;
        signed char *q = b + dec;
        signed char *r = b + size - 1;
        while ( p < q && *p == 0 ) {
            ++p;
        }
        while ( r > q && *r == 0 ) {
            --r;
        }
        dec = q - p;
        size = r - p + 1;
        std::memcpy(b, p, size);
    }

    const int sgn() const { return (sign) ? 1 : -1; }
    buff& abs() {
        sign = true;
        return *this;
    }
    buff& inv() {
        sign = !sign;
        return *this;
    }
    buff& neg() {
        sign = false;
        return *this;
    }

    //   +-- begin
    //   +-- mbegin
    //   |  +-- mend
    //   |  +-- d
    //   1234.5678
    //        |  +-- end
    //        |  +-- fend
    //        +-- fbegin  
    const signed char *begin()  const { return ( !size ) ? b : b + size - 1; }
    const signed char *end()    const { return b; }
    const signed char *mbegin() const { return begin(); }
    const signed char *mend()   const { return b + dec; }
    const signed char *fbegin() const { return ( !dec ) ? b : b + dec - 1; }
    const signed char *fend()   const { return end(); }

    const size_t mant_sz() const { return size - dec; }
    const size_t frac_sz() const { return dec; }

    // this - rhs:
    //  -1 if this <  rhs
    //   0    this == rhs
    //   1    this >  rhs
    // 
    // *** The compare for this is complicated due to the decimal point.
    // *** delaying implementation until I get this figured out but this
    // *** will need to be done.
    // int compare(const buff& rhs) const;
    // inline bool operator< (const buff& rhs) const { return compare(rhs) <  0; }
    // inline bool operator<=(const buff& rhs) const { return compare(rhs) <= 0; }
    // inline bool operator> (const buff& rhs) const { return compare(rhs) >  0; }
    // inline bool operator>=(const buff& rhs) const { return compare(rhs) <= 0; }
    // inline bool operator==(const buff& rhs) const { return compare(rhs) == 0; }


    buff operator+(const buff& rhs) const;
    buff operator-(const buff& rhs) const;
    buff& operator +=(const int rhs) {
        *this = *this + buff(rhs);
        return *this;
    }
    buff& nines();

    friend std::ostream& operator<<(std::ostream& os, const buff& o );
};

struct tuple {
    enum OpCode {
        NOP = 0,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        OP_CNT
    };

    #define NEG_NEG 0x00
    #define NEG_POS 0x01
    #define POS_NEG 0x02
    #define POS_POS 0x03

    static const char *OpNames[OpCode::OP_CNT];

    unsigned char mod;
    buff   lhs;
    buff   rhs;

    // construct a tuple of aligned numbers - 
    // with identical sizes - padded with zeros,
    // and identical fractions - padded with zeros
    tuple( const buff& l, const buff& r );
    tuple( const std::string& l, const std::string& r );
    virtual ~tuple();
    int compare(bool signless = false) const;
    tuple& swap();
    buff& add(buff& res, signed char *pcarry = nullptr);
    buff& sub(buff& res);
    buff& proc(OpCode opc, buff& res);
};

buff buff::operator+(const buff& rhs) const {
    buff res;
    return tuple(*this, rhs ).proc(tuple::ADD, res);
}

buff buff::operator-(const buff& rhs) const {
    buff res;
    return tuple(*this, rhs ).proc(tuple::SUB, res);
}

// convert buff to nine's complement
buff& buff::nines() {
    size_t cnt = size;
    signed char *p = b;
    for ( size_t cnt = size; cnt; --cnt, ++p )
        *p = 9 - *p;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const buff& o ) {
    os << o.size << " " << o.dec << " ";
    if ( o.size )
        std::cout << ("-+"[o.sign]);
    if ( o.size == o.dec ) {
        os << "0";
    } else {
        auto p = o.mbegin();
        while ( p >= o.mend() ) {
            char ch = (char)( '0' + *p-- );
            os << ch;
        }
    }
    if ( !o.dec ) {
        os << ".0";
    } else {
        os << '.';
        auto p = o.fbegin();
        while( p >= o.fend() ) {
            char ch = (char)('0' + *p--);
            os << ch;
        }
    }
    return os;
}

const char *tuple::OpNames[OpCode::OP_CNT] = {"NOP","ADD","SUB","MUL","DIV","MOD"};

int tuple::compare( bool signless ) const {
    if ( !signless ) {
        if ( mod == POS_NEG ) return  1;
        if ( mod == NEG_POS ) return -1;
    }
    // signs match, so compare buffers MSB to LSB
    auto *p = lhs.begin();
    auto *q = rhs.begin();
    while( p >= lhs.end() ) {
        if ( *p != *q )
            return *p - *q;
        p--;
        q--;
    }
    return 0;
}

// construct a tuple of aligned numbers - 
// with identical sizes - padded with zeros,
// and identical fractions - padded with zeros
tuple::tuple( const buff& l, const buff& r ) {
    mod = NEG_NEG;
    if (l.sign) mod |= POS_NEG;
    if (r.sign) mod |= NEG_POS;

    auto msz = std::max( l.mant_sz(), r.mant_sz() );
    auto fsz = std::max( l.frac_sz(), r.frac_sz() );
    auto siz = msz + fsz;
    lhs.reset( siz, fsz, &l );
    rhs.reset( siz, fsz, &r );
    if ( compare() < 0 )
        swap();
    std::cout <<"tup " << lhs << " " << rhs << std::endl;
}

tuple::tuple(const std::string& l, const std::string& r)
: tuple(buff(l),buff(r))
{}

tuple::~tuple() {
}

tuple& tuple::swap() {
    std::swap(lhs,rhs);
    return *this;
}

buff& tuple::add(buff& res, signed char *pcarry) {
    // increase size by 1 to account for final carry
    res.reset(lhs.size+1, lhs.dec);
    signed char carry(0);
    signed char *p = lhs.b;
    signed char *q = rhs.b;
    signed char *r = res.b;
    size_t cnt = lhs.size;
    while (cnt--) {
        signed char d = *p++ + *q++ + carry;
        *r++  = d % 10;
        carry = d / 10;
    }
    if ( carry )
        if ( pcarry != nullptr )
            *pcarry = carry;
        else
            *r++ = carry;
    res.trim();
    std::cout << "ADD " << lhs << " " << rhs << " " << res << std::endl;
    return res;
}

//https://www.eeeguide.com/bcd-subtraction/
buff& tuple::sub(buff& res) {
    signed char carry = 0;
    rhs.nines();
    add(res, &carry);
    if ( carry != 0 ) {
        // add the carry back in we're done
        res += carry;
        // auto tmp = res;
        // tuple(tmp, buff(carry)).add(res);
    } else {
        // no carry - so answer is in nines form and NEGATIVE
        res.nines().neg();
    }
    std::cout << "SUB " << lhs << " " << rhs << " " << res << std::endl;
    return res;
}

buff& tuple::proc(OpCode opc, buff& res) {
    switch( opc ) {
        case ADD:
            switch( mod ) {
                case POS_POS:
                    // a + b
                    return add(res);
                case NEG_NEG:
                    // -a + -b === -a - b === -(a + b)
                    return add(res).neg();
                case POS_NEG:
                    // a + -b === a - b
                    return sub(res);
                case NEG_POS:
                    // -a + b === b - a
                    return swap().sub(res);
            }
            break;
        case SUB:
            switch( mod ) {
                case POS_POS:
                    // a - b
                    return sub(res);
                case NEG_NEG:
                    // -a - -b === -a + b === b - a
                    return swap().sub(res);
                case POS_NEG:
                    // a - -b === a + b
                    return add(res);
                case NEG_POS:
                    // -a - b === -(a + b)
                    return add(res).neg();
            }
            break;
    }
    return res;
}

int main() {
    buff a("+00001234.43210000");
    std::cout << a << std::endl;
    buff b("-1234.4321");
    std::cout << b << std::endl;
    buff c("1234.4321-");
    std::cout << c << std::endl;
    buff d("1234.4321+");
    std::cout << d << std::endl;
    buff e;
    std::cout << e << std::endl;
    e.trim();
    std::cout << e << std::endl;
    buff f(".4321+");
    std::cout << f << std::endl;
    buff g("4321+");
    std::cout << g << std::endl;

    buff l("12345678.9");
    buff r("9.87654321");
    buff res;
    tuple q(l,r);   
    std::cout << q.add(res) << std::endl;
    std::cout << a.nines() << std::endl;
    std::cout << a.nines() << std::endl;
    std::cout << (buff( "100") + buff( "60")) << std::endl;
    std::cout << (buff( "100") + buff("-60")) << std::endl;
    std::cout << (buff("-100") + buff( "60")) << std::endl;
    std::cout << (buff("-100") + buff("-60")) << std::endl;
    std::cout << (buff( "100") - buff( "60")) << std::endl;
    std::cout << (buff( "100") - buff("-60")) << std::endl;
    std::cout << (buff("-100") - buff( "60")) << std::endl;
    std::cout << (buff("-100") - buff("-60")) << std::endl;
    std::cout << (buff(  "60") + buff( "100")) << std::endl;
    std::cout << (buff(  "60") + buff("-100")) << std::endl;
    std::cout << (buff( "-60") + buff( "100")) << std::endl;
    std::cout << (buff( "-60") + buff("-100")) << std::endl;
    std::cout << (buff(  "60") - buff( "100")) << std::endl;
    std::cout << (buff(  "60") - buff("-100")) << std::endl;
    std::cout << (buff( "-60") - buff( "100")) << std::endl;
    std::cout << (buff( "-60") - buff("-100")) << std::endl;

    return 0;
}