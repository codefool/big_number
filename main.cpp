#include "big_number"

int main(int argc, char **argv) {
    if ( argc < 3 ) {
        std::cout << "main lhs op rhs" << std::endl;
        exit(1);
    }

    big_number lhs(argv[1]);
    char *op (argv[2]);
    big_number rhs(argv[3]);
    big_number ans;

    switch( *op ) {
        case '+': ans = lhs + rhs; break;   // addition
        case '-': ans = lhs - rhs; break;   // subtraction
        case 'x':                           // multiplication
        case '*': ans = lhs * rhs; break;   // multiplication
        case '/': ans = lhs / rhs; break;   // int division
        case '%': ans = lhs % rhs; break;   // modulus
        case '^': ans = lhs ^ rhs; break;   // exponent
            std::cerr << "unsupported operator '" << *op << "'" << std::endl;
            exit(2);
    }

    std::cout << ans << std::endl;

    return 0;
}