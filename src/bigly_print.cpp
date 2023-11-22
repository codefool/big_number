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
    size_t cnt = obj.magn();
    if ( !cnt ) {
        os << '0';
    } else {
        auto itr = obj.crbegin();
        while (  itr != obj.crend() ) {
            if ( !(cnt-- % 3) && itr != obj.crbegin() ) {
                os << sep;
            }
            bigly::digit_t ch = ('0' + *itr++);
            os << std::hex << (char)(ch);
        }
        cnt = obj.frac();
        if ( cnt ) {
            os << dec;
            auto itr = obj.fcrbegin();
            while (  itr != obj.fcrend() ) {
                if ( !(cnt-- % 3) && itr != obj.crbegin() ) {
                    os << sep;
                }
                bigly::digit_t ch = ('0' + *itr++);
                os << std::hex << (char)(ch);
            }
        }
    }
    return os;
    
}
} // end namespace cflib
