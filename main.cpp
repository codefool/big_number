#include "bigly"

int main(int argc, char **argv) {
    if ( argc < 3 ) {
        std::cout << "main lhs op rhs" << std::endl;
        exit(1);
    }

    cflib::bigly lhs(argv[1]);
    std::string op(argv[2]);
    cflib::bigly rhs(argv[3]);
    cflib::bigly ans;

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

    std::cout << cflib::bigly::factorial(100) << std::endl;

    std::cout << cflib::bigly::fibonacci(100) << std::endl;

    cflib::bigly frac(100);
    frac.prefrac(1);
    frac.prefrac(2);
    frac.prefrac(3);
    frac.prefrac(4);
    std::cout << frac << std::endl;



    return 0;
}