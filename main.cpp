#include "big_number"

int main() {
    big_number a("123456789");

    auto p = a.get();
    size_t l = a.size();
    while( l-- )
        std::cout << (char)(*p++ + '0');
    std::cout << std::endl;


    return 0;
}