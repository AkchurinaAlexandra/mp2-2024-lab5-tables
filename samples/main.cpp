#include <iostream>
#include <table.h>
#include <Polynomial.h>

void printMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Add a polynomial to the table\n";
    std::cout << "2. Find polynomial in the table by key\n";
    std::cout << "3. Extract a polynomial by key\n";
    std::cout << "4. Arithmetic with polynomials\n";
    std::cout << "5. Exit\n";
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

Polynomial enter_polinom() {
    std::cout << "Enter polynomial:\n";
    std::cin.ignore();
    std::string poly_str;
    std::getline(std::cin, poly_str);
    return Polynomial(poly_str);
}


Polynomial enter_polinom_or_from_table(Table<std::string, Polynomial>& table) {
    std::cout << "Do you want to enter a polynomial or get it from the table?\n"
                 "\t1. Enter polynomial\n"
                 "\t2. Get polynomial from the table\n";
    while (true) {
        int opt;
        std::cin >> opt;
        switch (opt){
            case 1:
                return enter_polinom();
            case 2: {
                std::string key;
                std::cout << "Enter key of polynomial:\n";
                std::cin >> key;
                try {
                    Polynomial poly = table.get(key);
                    std::cout << "The polynomial: " << poly.toString() << "\n";
                    return poly;
                } catch (const std::runtime_error &e) {
                    std::cout << "Polynomial not found in the table.\n";
                }
            }
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }

}

void doPolynomialOperations(Table<std::string, Polynomial>& table) {
    try {
        std::cout << "Choose an operation\n"
                     "\t1. Add two polynomials\n"
                     "\t2. Subtract two polynomials\n"
                     "\t3. Multiply two polynomials\n"
                     "\t4. Multiply a polynomial by a number\n";
        int choice;
        std::cin >> choice;
        std::cout << "Enter the first polynomial:\n";
        Polynomial p1 = enter_polinom_or_from_table(table);
        std::cout << "Enter the second polynomial:\n";
        Polynomial p2 = enter_polinom_or_from_table(table);
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
        std::cout << "Do you want to save the result in the table?\n"
                     "\t1. Yes\n"
                     "\t2. No\n";
        int opt;
        std::cin >> opt;
        if (opt == 1) {
            std::string key;
            std::cout << "Enter key:\n";
            std::cin >> key;
            table.put(key, result);
            std::cout << "Polynomial saved in the table.\n";
        } else if (opt == 2) {
            std::cout << "Polynomial not saved in the table.\n";
        } else {
            std::cerr << "Invalid option. Choose 1 or 2\n";
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    Table<std::string, Polynomial> table;
    int choice;

    do {
        printMenu();
        std::cout << "Choose an action: ";
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string key;
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter polynomial:\n";

                std::cin.ignore();
                std::string poly_str;
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
                    std::cout << "The polynomial: " << poly.toString() << "\n";
                } catch (const std::runtime_error &e) {
                    std::cout << "Polynomial not found in the table.\n";
                }
                break;
            }
            case 3: {
                std::string key;
                std::cout << "Enter key: ";
                std::cin >> key;
                try {
                    table.extract(key);
                    std::cout << "Polynomial is extracted.\n";
                } catch (const std::runtime_error &e) {
                    std::cout << "Polynomial not found in the table.\n";
                }
                break;
            }
            case 4: {
                doPolynomialOperations(table);
                break;
            }
            case 5:
                std::cout << "Exit.\n";
                break;
            default:
                std::cout << "Unknown operation.\n";
        }
    } while (choice != 5);

    return 0;
}