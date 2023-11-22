/*
 * bigly - library for arbitrarily large numbers
 *
 * Copyright (C) 2023 Garyl Hester. All rights reserved.
 * 
 * This project lives at https://github.com/codefool/bigly
 *
 */
#include "bigly"

namespace cflib {
std::ostream& operator<<(std::ostream& os, const bigly& obj) {
    char sep = '\'';
    char dec = '.';
    if (obj.sign() < 0 ) {
        os << '-';
    }
    size_t cnt = obj.mant();
    if ( !cnt ) {
        os << '0';
    } else {
        auto itr = obj.mend();
        while (  itr >= obj.mbegin() ) {
            if ( !(cnt-- % 3) && itr != obj.mend() ) {
                os << sep;
            }
            char ch = ('0' + *itr--);
            os << ch;
        }
        cnt = obj.frac();
        if ( cnt ) {
            os << dec;
            auto itr = obj.fend();
            while (  itr >= obj.fbegin() ) {
                if ( !(cnt-- % 3) && itr != obj.fend() ) {
                    os << sep;
                }
                char ch = ('0' + *itr--);
                os << ch;
            }
        }
    }
    return os;
    
}
} // end namespace cflib
