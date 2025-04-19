#include "Monomial.h"
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <cmath>

const short Monomial::DEFAULT_PRECISION = 7;

Monomial::Monomial() : powers(0), coeff(0) {}

Monomial::Monomial(double coeff, short powers) : coeff(coeff), powers(powers) {}

Monomial::Monomial(const std::string &s) : Monomial() {
    size_t pos = 0;
    coeff = 1;
    std::string coeff_str;
    for (; pos < s.size(); ++pos) {
        if (!isdigit(s[pos]) && s[pos] != 'E' && s[pos] != 'e'
                && s[pos] != '+' && s[pos] != '-' && s[pos] != '.') {
            break;
        }
        if ((s[pos] == '+' || s[pos] == '-') &&
            (!coeff_str.empty() || coeff_str.back() != 'E' || coeff_str.back() !='e')) {
            break;
        }
        coeff_str += s[pos];
    }
    if (!coeff_str.empty()) {
        coeff = custom_stod(coeff_str);
    }
    for (; pos < s.size(); ++pos) {
        if (isspace(s[pos])) {
            continue;
        }
        char var = s[pos];
        short power = 1;
        while(pos + 1 < s.size() && isspace(s[pos + 1])) {
            pos++;
        }
        if (pos + 1 < s.size() && isdigit(s[pos + 1])) {
            power = s[pos + 1] - '0';
            pos++;
        }
        switch (var) {
            case 'X':
            case 'x':
                setX(getX() + power);
                break;
            case 'Y':
            case 'y':
                setY(getY() + power);
                break;
            case 'Z':
            case 'z':
                setZ(getZ() + power);
                break;
            default:
                throw std::runtime_error("Unknown variable "
                                            + std::to_string(var)
                                            + ". Make sure you are using only x, y, z"
                                              "(capital or lowercase) and single-digit powers.");
        }
    }
}

void Monomial::setX(short value) {
    if (value < 0 || value > 9) {
        throw std::runtime_error("Power of variable in monomial must be from 0 to 9.");
    }
    powers = powers % 100 + value * 100;
}

void Monomial::setY(short value) {
    if (value < 0 || value > 9) {
        throw std::runtime_error("Power of variable in monomial must be from 0 to 9.");
    }
    powers = powers - (powers / 10) % 10 * 10 + value * 10;
}

void Monomial::setZ(short value) {
    if (value < 0 || value > 9) {
        throw std::runtime_error("Power of variable in monomial must be from 0 to 9.");
    }
    powers = powers - powers % 10 + value;
}

void Monomial::setCoeff(double value) { coeff = value; }

short Monomial::getX() const {
    return powers / 100;
}

short Monomial::getY() const {
    return (powers / 10) % 10;
}

short Monomial::getZ() const {
    return powers % 10;
}

bool Monomial::isZero(short precision) const {
    if (fabs(coeff) < pow(10, -precision)) {
        return true;
    }
    return false;
}

std::string Monomial::toString(bool custom_precision, short precision) const {
    std::string result;
    if (isZero(precision)) {
        return "0";
    }
    if (powers == 0 || fabs(coeff - 1) >= pow(10, -precision)) {
        if (custom_precision) {
            std::ostringstream out;
            out << std::fixed << std::setprecision(precision) << coeff;
            result += out.str();
        } else {
            std::ostringstream out;
            out << std::defaultfloat << coeff;
            result += out.str();
        }
    }
    short px = getX();
    short py = getY();
    short pz = getZ();
    if (px) {
        result.push_back('x');
        if (px > 1) {
            result.push_back('0' + px);
        }
    }
    if (py) {
        result.push_back('y');
        if (py > 1) {
            result.push_back('0' + py);
        }
    }
    if (pz) {
        result.push_back('z');
        if (pz > 1) {
            result.push_back('0' + pz);
        }
    }
    return result;
}

/*
 * Compares powers of monomials with respect to the count of non-zero powers
 * (if equal, compares powers as two tuples)
 * (1, 0, 1) < (1, 1, 0)
 * (0, 2, 3) < (1, 1, 1)
 * (1, 2, 3) < (1, 3, 1)
 */
bool operator<(const Monomial &lhs, const Monomial &rhs) {
    short lpx = lhs.getX();
    short lpy = lhs.getY();
    short lpz = lhs.getZ();
    short cl = (lpx > 0) + (lpy > 0) + (lpz > 0);
    short rpx = rhs.getX();
    short rpy = rhs.getY();
    short rpz = rhs.getZ();
    short cr = (rpx > 0) + (rpy > 0) + (rpz > 0);
    if (cl != cr) {
        return cl < cr;
    }
    if (lpx != rpx) {
        return lpx < rpx;
    }
    if (lpy != rpy) {
        return lpy < rpy;
    }
    if (lpz != rpz) {
        return lpz < rpz;
    }
    return false;
}

bool operator>(const Monomial &lhs, const Monomial &rhs) {
    return rhs < lhs;
}

bool operator==(const Monomial &lhs, const Monomial &rhs) {
    return !(lhs < rhs) && !(rhs < lhs);
}

Monomial operator+(const Monomial &lhs, const Monomial &rhs) {
    if (lhs.powers != rhs.powers) {
        throw std::runtime_error("Cannot sum monomials with different powers");
    }
    return Monomial(lhs.coeff + rhs.coeff, lhs.powers);
}


Monomial operator-(const Monomial &lhs, const Monomial &rhs) {
    return lhs + (-1) * rhs;
}
Monomial operator*(const Monomial &lhs, const Monomial &rhs) {
    Monomial result = Monomial();
    result.setCoeff(lhs.coeff * rhs.coeff);
    result.setX(lhs.getX() + rhs.getX());
    result.setY(lhs.getY() + rhs.getY());
    result.setZ(lhs.getZ() + rhs.getZ());
    return result;
}

Monomial operator*(double s, const Monomial &m) {
    return Monomial(s * m.coeff, m.powers);
}

Monomial operator*(const Monomial &m, double s) {
    return Monomial(s * m.coeff, m.powers);
}

double Monomial::custom_stod(const std::string &s) {
    size_t e_pos = s.find_first_of("eE");
    std::string mantissa_str = s;
    std::string exponent_str = "0";

    if (e_pos != std::string::npos) {
        mantissa_str = s.substr(0, e_pos);
        exponent_str = s.substr(e_pos + 1);
    }

    double mantissa = 0.0;
    double factor = 1.0;
    bool decimal_point = false;
    bool negative = false;
    size_t i = 0;

    if (mantissa_str[i] == '-') {
        negative = true;
        i++;
    } else if (mantissa_str[i] == '+') {
        i++;
    }

    for (; i < mantissa_str.size(); i++) {
        if (mantissa_str[i] == '.') {
            decimal_point = true;
            continue;
        }
        if (mantissa_str[i] < '0' || mantissa_str[i] > '9') {
            throw std::invalid_argument("Invalid character in mantissa");
        }
        if (decimal_point) {
            factor /= 10.0;
            mantissa += (mantissa_str[i] - '0') * factor;
        } else {
            mantissa = mantissa * 10.0 + (mantissa_str[i] - '0');
        }
    }

    if (negative) {
        mantissa = -mantissa;
    }

    int exponent = 0;
    negative = false;
    i = 0;

    if (exponent_str[i] == '-') {
        negative = true;
        i++;
    } else if (exponent_str[i] == '+') {
        i++;
    }

    for (; i < exponent_str.size(); i++) {
        if (!isdigit(exponent_str[i])) {
            throw std::invalid_argument("Invalid character in exponent");
        }
        exponent = exponent * 10 + (exponent_str[i] - '0');
    }

    if (negative) {
        exponent = -exponent;
    }

    return mantissa * std::pow(10, exponent);
}

bool Monomial::getSign(short precision) const {
    return (coeff < -pow(10, -precision));
}




