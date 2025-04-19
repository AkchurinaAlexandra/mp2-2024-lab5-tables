#include <iostream>
#include <table.h>
#include <Polynomial.h>

void printMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Add a polynomial to the table\n";
    std::cout << "2. Find polynomial in the table by key\n";
    std::cout << "3. Extract a polynomial by key\n";
    std::cout << "4. Arithmetic with polynomials\n";
    std::cout << "5. Exit\n\n";
}

void printPolinomsFormat() {
    std::cout << "Input format:\n"
                 "1. Each monomial is a string of the form: KxAyBzC:\n"
                 "\t K - coefficient (double)\n"
                 "\t A, B, C - power of each variable (non-negative integers)\n"
                 "2. Monomials are separated by '+' or '-'\n"
                 "3. Spaces in monomials are not allowed\n"
                 "4. Maximal power of each variable is 9\n"
                 "5. Resulting polynomial cannot have power of a variable more than 9\n"
                 "6. Example of a polynomial: 2x3y4z1 + 3x2y3z4 - 4x3y4z5\n\n";
}

bool operator==(const Polynomial &lhs, const Polynomial &rhs) {
    return lhs.toString() == rhs.toString();
}

void doPolynomialOperations() { // from lab polinoms
    try {
        std::cout << "Choose an operation\n"
                     "\t1. Add two polynomials\n"
                     "\t2. Subtract two polynomials\n"
                     "\t3. Multiply two polynomials\n"
                     "\t4. Multiply a polynomial by a number\n";
        int choice;
        std::cin >> choice;
        std::string s1, s2;
        std::cout << "Enter the first polynomial:\n";
        std::cin.ignore();
        std::getline(std::cin, s1);
        Polynomial p1(s1);
        std::cout << "Enter the second polynomial:\n";
        std::getline(std::cin, s2);
        Polynomial p2(s2);
        Polynomial result;
        switch (choice) {
            case 1:
                result = p1 + p2;
                break;
            case 2:
                result = p1 - p2;
                break;
            case 3:
                result = p1 * p2;
                break;
            case 4:
                double s;
                std::cout << "Enter the number:\n";
                std::cin >> s;
                result = s * p1;
                break;
            default:
                std::cerr << "Invalid choice of an operation.\n";
        }
        std::cout << "result = " << result.toString() << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    Table<std::string, Polynomial> table;
    int choice;
    printPolinomsFormat();
    do {
        printMenu();
        std::cout << "Choose an action: ";
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string key;
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter polynomial: ";
                std::string poly_str;
                std::cin.ignore();
                std::getline(std::cin, poly_str);
                Polynomial poly(poly_str);
                table.put(key, poly);
                break;
            }
            case 2: {
                std::string key;
                std::cout << "Enter key of polynomial: ";
                std::cin >> key;
                try {
                    Polynomial poly = table.get(key);
                    std::cout << "\nThe polynomial: " << poly.toString() << "\n\n";
                } catch (const std::runtime_error &e) {
                    std::cout << "\nPolynomial not found in the table.\n\n";
                }
                break;
            }
            case 3: {
                std::string key;
                std::cout << "Enter key: ";
                std::cin >> key;
                try {
                    table.extract(key);
                    std::cout << "\nPolynomial is extracted.\n";
                } catch (const std::runtime_error &e) {
                    std::cout << "\nPolynomial not found in the table.\n\n";
                }
                break;
            }
            case 4: {
                doPolynomialOperations();
                break;
            }
            case 5:
                std::cout << "\nExit.\n";
                break;
            default:
                std::cout << "\nUnknown operation.\n";
        }
    } while (choice != 5);

    return 0;
}