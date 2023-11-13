#include "big_number"

int main(int argc, char **argv) {
    if ( argc < 3 ) {
        std::cout << "main lhs op rhs" << std::endl;
        exit(1);
    }

    big_number lhs(argv[1]);
    std::string op(argv[2]);
    big_number rhs(argv[3]);
    big_number ans;

    std::string compops("==!=<><=>=co");

    if ( compops.find(op) != std::string::npos ) {
        if ( op == "==" ) {
            std::cout << op << ' ' << (lhs == rhs) << std::endl;
        } else if ( op == "!=" ) {
            std::cout << op << ' ' << (lhs != rhs) << std::endl;
        } else if ( op == "<" ) {
            std::cout << op << ' ' << (lhs < rhs) << std::endl;
        } else if ( op == "<=" ) {
            std::cout << op << ' ' << (lhs <= rhs) << std::endl;
        } else if ( op == ">" ) {
            std::cout << op << ' ' << (lhs > rhs) << std::endl;
        } else if ( op == ">=" ) {
            std::cout << op << ' ' << (lhs >= rhs) << std::endl;
        } else if ( op == "co" ) {
            std::cout << op << ' ' << lhs.compare(rhs) << std::endl;
        }
        return 0;
    }
    
    switch( op.front() ) {
        case '+': ans = lhs + rhs; break;   // addition
        case '-': ans = lhs - rhs; break;   // subtraction
        case 'x':                           // multiplication
        case '*': ans = lhs * rhs; break;   // multiplication
        case '/': ans = lhs / rhs; break;   // int division
        case '%': ans = lhs % rhs; break;   // modulus
        case '^': ans = lhs ^ rhs; break;   // exponent
        default:
            std::cerr << "unsupported operator '" << op << "'" << std::endl;
            exit(2);
    }

    std::cout << ans << std::endl;

    std::cout << big_number::factorial(100) << std::endl;

    std::cout << big_number::fibonacci(100) << std::endl;

    return 0;
}