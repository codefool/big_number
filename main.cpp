#include "big_number"

int main() {
    big_number a("1234567890");
    big_number b("123456789");

    std::cout << (a < b) << std::endl;
    std::cout << (a > b) << std::endl;
    std::cout << (a == b) << std::endl;

    auto c = a + b;
    auto d = a - b;

    std::cout << d << std::endl;



    return 0;
}