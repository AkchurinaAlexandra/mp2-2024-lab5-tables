#include "Polynomial.h"

Polynomial::Polynomial() : precision(Monomial::DEFAULT_PRECISION), custom_precision(false) {}

Polynomial::Polynomial(const std::string &s) : Polynomial() {
    size_t pos = 0;
    bool sgn = false;
    bool any_monomials_expected = false;
    while (pos < s.size() && (s[pos] == '+' || s[pos] == '-' || isspace(s[pos]))) {
        if (s[pos] == '-') {
            sgn ^= 1;
            any_monomials_expected = true;
        }
        if (s[pos] == '+') {
            any_monomials_expected = true;
        }
        pos++;
    }
    while (pos < s.size()) {
        size_t start = pos;
        while (pos < s.size() && s[pos] != '+' && s[pos] != '-') {
            pos++;
        }
        double multiplier = 1.0;
        if (sgn) {
            multiplier = -1.0;
            sgn = false;
        }
        monomials.push_back(multiplier * Monomial(s.substr(start, pos - start)));
        while (pos < s.size() && (s[pos] == '+' || s[pos] == '-' || isspace(s[pos]))) {
            if (s[pos] == '-') {
                sgn ^= 1;
            }
            pos++;
        }
    }

    if (monomials.empty() && any_monomials_expected) {
        throw std::runtime_error("Empty monomial.");
    }
    sort();
    removeDuplicates();
}


void Polynomial::pushNewMonomial(const Monomial &m, bool sorted) {
    if (monomials.empty()) {
        monomials.push_back(m);
        return;
    }
    Monomial& last = monomials.back();
    monomials.push_back(m);
    if (!sorted) {
        sort();
    }
    if (last == m) {
        last = last + m;
    }
}

void Polynomial::setPrecision(int new_precision) {
    this->precision = new_precision;
    custom_precision = true;
}

void Polynomial::removeDuplicates() {
    List<Monomial> new_monomials;
    Monomial prev;
    while (!monomials.empty()) {
        Monomial current = monomials.extract();
        if (prev == current) {
            prev = prev + current;
        } else {
            if (!prev.isZero()) {
                new_monomials.push_back(prev);
            }
            prev = current;
        }
    }
    if (!prev.isZero()) {
        new_monomials.push_back(prev);
    }
    monomials = new_monomials;
}

Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result;
    result.monomials = merge_sorted_lists(lhs.monomials, rhs.monomials, false);
    result.removeDuplicates();
    return result;
}

Polynomial operator*(double s, const Polynomial &m) {
    Polynomial result;
    List<Monomial> new_monomials = m.monomials;
    while (!new_monomials.empty()) {
        Monomial current = new_monomials.extract();
        result.pushNewMonomial(current * s);
    }
    return result;
}

Polynomial operator*(const Polynomial &m, double s) {
    return s * m;
}

Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
    return lhs + (-1.0) * rhs;
}

Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result;
    List<Monomial> new_monomials;
    List<Monomial> lhs_monomials = lhs.monomials;
    List<Monomial> rhs_monomials = rhs.monomials;
    while (!lhs_monomials.empty()) {
        Monomial lhs_current = lhs_monomials.extract();
        while (!rhs_monomials.empty()) {
            Monomial rhs_current = rhs_monomials.extract();
            new_monomials.push_back(lhs_current * rhs_current);
        }
        rhs_monomials = rhs.monomials;
    }
    result.monomials = new_monomials;
    result.sort();
    result.removeDuplicates();
    return result;
}

std::string Polynomial::toString() const {
    std::string result;
    if (monomials.empty()) {
        return "0";
    }
    List<Monomial> new_monomials = monomials;
    bool sgn = false;
    while (!new_monomials.empty()) {
        Monomial current = new_monomials.extract();
        if (sgn) {
            current = current * (-1.0);
        }
        result += current.toString(custom_precision, precision);
        if (!new_monomials.empty()) {
            if (new_monomials.get_first().getSign()) {
                result += " - ";
                sgn = true;
            } else {
                result += " + ";
                sgn = false;
            }
        }
    }
    return result;
}

void Polynomial::sort() {
    merge_sort(monomials, false);
}
