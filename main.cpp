#include "big_number"

int main() {
    big_number n0("1234567890");
    big_number n1("123456789");
    big_number n2("-123456789");


    auto c = n0 + n1;
    std::cout << c << std::endl;
    c = n0 - n1;
    std::cout << c << std::endl;
    c = n0 + n2;
    std::cout << c << std::endl;
    c = n2 + n1;
    std::cout << c << std::endl;
    c = big_number("-12345") - big_number::ONE;
    std::cout << c << std::endl;
    c = big_number("-12345") - big_number::ONE.negate();
    std::cout << c << std::endl;
    c = big_number("-1") + big_number("-10");
    std::cout << c << std::endl;
    c = big_number("-1") - big_number("-10");
    std::cout << c << std::endl;



    return 0;
}