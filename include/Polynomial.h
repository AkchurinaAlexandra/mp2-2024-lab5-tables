#include "Monomial.h"
#include "List.h"

class Polynomial {
 private:
    List<Monomial> monomials;
    short precision = Monomial::DEFAULT_PRECISION;
    bool custom_precision;
 public:
    Polynomial();
    explicit Polynomial(const std::string& s);
    void pushNewMonomial(const Monomial& m, bool sorted = true);
    void setPrecision(int new_precision);
    void sort();
    void removeDuplicates();
    [[nodiscard]] std::string toString() const;

    friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);
    friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs);
    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);
    friend Polynomial operator*(double s, const Polynomial& m);
    friend Polynomial operator*(const Polynomial& m, double s);
};