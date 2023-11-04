#include "big_number"

std::ostream& operator<<(std::ostream& os, big_number& obj) {
    if (obj.sign() < 0 ) {
        os << '-';
    }
    size_t idx = obj.size();
    while ( idx ) {
        big_number::digit_t ch = obj[--idx];
        os << (char)('0' + ch);
    }
    return os;
    
}
