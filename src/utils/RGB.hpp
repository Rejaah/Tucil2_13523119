#ifndef RGB_HPP
#define RGB_HPP

#include <cstdint>
#include <cmath>
#include <iostream>

class RGB {
private:
    uint8_t r, g, b;

public:
    RGB() : r(0), g(0), b(0) {}
    RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    uint8_t getR() const { return r; }
    uint8_t getG() const { return g; }
    uint8_t getB() const { return b; }

    void setR(uint8_t value) { r = value; }
    void setG(uint8_t value) { g = value; }
    void setB(uint8_t value) { b = value; }

    double distanceTo(const RGB& other) const {
        int dr = int(r) - int(other.r);
        int dg = int(g) - int(other.g);
        int db = int(b) - int(other.b);
        return std::sqrt(dr * dr + dg * dg + db * db);
    }

    RGB operator+(const RGB& other) const {
        return RGB(r + other.r, g + other.g, b + other.b);
    }

    RGB operator/(int scalar) const {
        return RGB(r / scalar, g / scalar, b / scalar);
    }

    friend std::ostream& operator<<(std::ostream& os, const RGB& color) {
        os << "(" << int(color.r) << ", " << int(color.g) << ", " << int(color.b) << ")";
        return os;
    }   
};

#endif
