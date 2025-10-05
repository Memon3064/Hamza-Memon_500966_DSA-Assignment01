#include "polynomial.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void Polynomial::insertTerm(int, int) {}
string Polynomial::toString() const { return ""; }
Polynomial Polynomial::add(const Polynomial&) const { return *this; }
Polynomial Polynomial::multiply(const Polynomial&) const { return *this; }
Polynomial Polynomial::derivative() const { return *this; }

class PolynomialImpl : public Polynomial {
private:
    std::vector<int> coefficients;
    std::vector<int> exponents;

    void sortTerms() {
        for (size_t i = 0; i < exponents.size(); i++) {
            for (size_t j = i + 1; j < exponents.size(); j++) {
                if (exponents[j] > exponents[i]) {
                    std::swap(exponents[i], exponents[j]);
                    std::swap(coefficients[i], coefficients[j]);
                }
            }
        }
    }

public:
    PolynomialImpl() {}

    void insertTerm(int coefficient, int exponent) override {
        if (exponent < 0 || coefficient == 0) return;

        for (size_t i = 0; i < exponents.size(); i++) {
            if (exponents[i] == exponent) {
                coefficients[i] += coefficient;
                if (coefficients[i] == 0) {
                    coefficients.erase(coefficients.begin() + i);
                    exponents.erase(exponents.begin() + i);
                }
                return;
            }
        }
        coefficients.push_back(coefficient);
        exponents.push_back(exponent);
        sortTerms();
    }

    string toString() const override {
        if (coefficients.empty()) return "0";

        string result = "";
        for (size_t i = 0; i < coefficients.size(); i++) {
            int c = coefficients[i];
            int e = exponents[i];
            if (c == 0) continue;
            if (i > 0) {
                if (c > 0) result += " + ";
                else result += " - ";
            } else {
                if (c < 0) result += "-";
            }

            int absCoeff = (c < 0) ? -c : c;
            if (e == 0) {
                result += to_string(absCoeff);
            } else if (e == 1) {
                if (absCoeff != 1) result += to_string(absCoeff);
                result += "x";
            } else {
                if (absCoeff != 1) result += to_string(absCoeff);
                result += "x^" + to_string(e);
            }
        }
        return result;
    }

    PolynomialImpl addPoly(const PolynomialImpl &other) const {
        PolynomialImpl result;
        for (size_t i = 0; i < coefficients.size(); i++)
            result.insertTerm(coefficients[i], exponents[i]);
        for (size_t i = 0; i < other.coefficients.size(); i++)
            result.insertTerm(other.coefficients[i], other.exponents[i]);
        return result;
    }

    PolynomialImpl multiplyPoly(const PolynomialImpl &other) const {
        PolynomialImpl result;
        for (size_t i = 0; i < coefficients.size(); i++) {
            for (size_t j = 0; j < other.coefficients.size(); j++) {
                int newCoeff = coefficients[i] * other.coefficients[j];
                int newExp = exponents[i] + other.exponents[j];
                result.insertTerm(newCoeff, newExp);
            }
        }
        return result;
    }

    PolynomialImpl derivativePoly() const {
        PolynomialImpl result;
        for (size_t i = 0; i < coefficients.size(); i++) {
            if (exponents[i] > 0) {
                result.insertTerm(coefficients[i] * exponents[i], exponents[i] - 1);
            }
        }
        return result;
    }

    Polynomial add(const Polynomial&) const override { return *this; }
    Polynomial multiply(const Polynomial&) const override { return *this; }
    Polynomial derivative() const override { return *this; }
};

int main() {
    PolynomialImpl p1;
    p1.insertTerm(3, 4);
    p1.insertTerm(2, 2);
    p1.insertTerm(-1, 1);
    p1.insertTerm(5, 0);

    PolynomialImpl p2;
    p2.insertTerm(1, 4);
    p2.insertTerm(1, 0);

    PolynomialImpl sum = p1.addPoly(p2);

    PolynomialImpl p3;
    p3.insertTerm(2, 1);

    PolynomialImpl prod = sum.multiplyPoly(p3);
    PolynomialImpl deriv = p1.derivativePoly();

    cout << "p1.toString(): "   << p1.toString()   << "\n";
    cout << "sum.toString(): "  << sum.toString()  << "\n";
    cout << "prod.toString(): " << prod.toString() << "\n";
    cout << "deriv.toString(): "<< deriv.toString() << "\n";
    return 0;
}
