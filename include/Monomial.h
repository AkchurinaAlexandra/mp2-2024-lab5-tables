#include <string>

class Monomial {
 private:
    double coeff;
    short powers;
    static double custom_stod(const std::string& s);
 public:
    static const short DEFAULT_PRECISION;
    Monomial();
    Monomial(const std::string& s);
    Monomial(double coeff, short powers);
    void setX(short value);
    void setY(short value);
    void setZ(short value);
    void setCoeff(double value);
    short getX() const;
    short getY() const;
    short getZ() const;
    bool getSign(short precision = DEFAULT_PRECISION) const;
    bool isZero(short precision = DEFAULT_PRECISION) const;
    std::string toString(bool custom_precision = false, short precision = DEFAULT_PRECISION) const;
    friend bool operator<(const Monomial& lhs, const Monomial& rhs);
    friend bool operator>(const Monomial& lhs, const Monomial& rhs);
    friend bool operator==(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator+(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator-(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator*(double s, const Monomial& m);
    friend Monomial operator*(const Monomial& m, double s);
};