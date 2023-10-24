#include "big_number"

int main() {
    big_number a("123456789123456789123456789123456789123456789123456789123456789");

    std::cout << "hello, world" << std::endl;
    auto p = a.get();
    size_t l = a.size();
    while( l-- ) {
        char c = (char)(*p++) + '0';
        std::cout << c;
    }
    std::cout << std::endl;


    return 0;
}