#include "big_number"

int main() {
    big_number a("+1234567890");
    big_number b("-1234567890");

    std::cout << (a < b) << std::endl;
    std::cout << (a > b) << std::endl;
    std::cout << (a == b) << std::endl;

    std::cout << "hello, world" << std::endl;
    size_t l = a.size();
    auto p = a.get() + l - 1;
    while( l-- ) {
        char c = (char)(*p--) + '0';
        std::cout << c;
    }
    std::cout << std::endl;


    return 0;
}