/*
 * big_number - library for arbitrarily large numbers
 *
 * Copyright (C) 2023 Garyl Hester. All rights reserved.
 * 
 * This project lives at https://github.com/codefool/big_number
 *
 */
#include "big_number"

namespace cflib {
std::ostream& operator<<(std::ostream& os, const big_number& obj) {
    if (obj.sign() < 0 ) {
        os << '-';
    }
    size_t idx = obj.magn();
    if ( !idx ) {
        os << '0';
    } else {
        auto itr = obj.crbegin();
        while (  itr != obj.crend() ) {
            big_number::digit_t ch = ('0' + *itr++);
            os << (char)(ch);
        }
    }
    return os;
    
}
} // end namespace cflib
