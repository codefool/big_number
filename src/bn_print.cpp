#include "big_number"

std::ostream& operator<<(std::ostream& os, big_number& obj) {
    if (obj.sign() < 0 ) {
        os << '-';
    }
    size_t idx = obj.magn();
    if ( !idx ) {
        os << '0';
    } else {
        auto itr = obj.rbegin();
        while (  itr != obj.rend() ) {
            big_number::digit_t ch = ('0' + *itr++);
            os << (char)(ch);
        }
    }
    return os;
    
}
