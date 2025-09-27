#include <iostream>
#include <algorithm>

//func == &func
//reinterpret_cast<int*>(...)

//-h   help
// man ...       == manual
// make main    or     make

//g++ -E main.cpp -o pre_main.cpp                -E == только текстовая подстановка
//                -c               -DN=100        -D == define     N == variable_name        -c == без линковки





class Rational final {
public:
    Rational(int numerator, int denominator);
    Rational(const Rational& other);

    double toDouble() const;
    static int gcd(int a, int b);
    void normalize();

    Rational& operator=(const Rational& other){
        if (this != &other) {
            m_numerator = other.m_numerator;
            m_denominator = other.m_denominator;
        }
        return *this;
    }
    Rational operator-() const {
        return Rational(-m_numerator, m_denominator);
    }
    Rational operator+() const {
        return *this;
    }

    Rational& operator+=(const Rational& other) {
        m_numerator = m_numerator * other.m_denominator + other.m_numerator * m_denominator;
        m_denominator = m_denominator * other.m_denominator;
        normalize();
        return *this;
    }
    Rational& operator*=(const Rational& other) {
        m_numerator = m_numerator * other.m_numerator;
        m_denominator = m_denominator * other.m_denominator;
        normalize();
        return *this;
    }
    Rational& operator/=(const Rational& other) {
        m_numerator = m_numerator * other.m_denominator;
        m_denominator = m_denominator * other.m_numerator;
        normalize();
        return *this;
    }
    Rational& operator-=(const Rational& other) {
        return (*this+=(-other));
    }
    
    Rational& operator+=(const int& other) {
        return *this += Rational(other, 1);
    }
    Rational& operator*=(const int& other) {
        return *this *= Rational(other, 1);
    }
    Rational& operator/=(const int& other) {
        return *this /= Rational(other, 1);
    }
    Rational& operator-=(const int& other) {
        return *this -= Rational(other, 1);
    }

    Rational operator+(const Rational& other) {
        Rational buf = *this;
        buf += other;
        buf.normalize();
        return buf;
    }
    Rational operator*(const Rational& other) {
        Rational buf = *this;
        buf *= other;
        buf.normalize();
        return buf;
    }
    Rational operator/(const Rational& other) {
        Rational buf = *this;
        buf /= other;
        buf.normalize();
        return buf;
    }
    Rational operator-(const Rational& other) {
        return (*this+(-other));
    }
    
    Rational operator+(const int& other) {
        return *this + Rational(other, 1);
    }
    Rational operator*(const int& other) {
        return *this * Rational(other, 1);
    }
    Rational operator/(const int& other) {
        return *this / Rational(other, 1);
    }
    Rational operator-(const int& other) {
        return *this - Rational(other, 1);
    }
    
    


private:
    friend bool operator<(const Rational &, const Rational &);

    int m_numerator = 0;
    int m_denominator = 1;
};
    
    
 int Rational::gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

void Rational::normalize() {
        if (m_denominator < 0) {
            m_numerator = -m_numerator;
            m_denominator = -m_denominator;
        }
        
        int gcd_val = gcd(std::abs(m_numerator), m_denominator);
        m_numerator /= gcd_val;
        m_denominator /= gcd_val;
    }

Rational::Rational(int numerator, int denominator){
    m_numerator = numerator;
    m_denominator = denominator;
}

Rational::Rational(const Rational& other){
    m_numerator = other.m_numerator;
    m_denominator = other.m_denominator;
}











int main() {
    Rational a(1,1);
    a + 1;
    //1 + a;

    return 0;
}