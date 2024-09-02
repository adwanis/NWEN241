#include <iostream>

namespace Complex {
    class complex  {
    private:
        int a;
        int b;

    public:
        // Default constructor
        complex() : a(1), b(1) {}

        // Parameterized constructor
        complex(int _a, int _b) : a(_a), b(_b) {}

        // Getter functions
        int geta()  { return a; }
        int getb()  { return b; }
    };
}

int main() {
    // Declare complex numbers using default and parameterized constructors
    Complex::complex c1;
    Complex::complex c2(5, 10);

    // Display the values of both complex numbers
    std::cout << "Complex number 1: " << c1.geta() << " + " << c1.getb() << "i\n";
    std::cout << "Complex number 2: " << c2.geta() << " + " << c2.getb() << "i\n";

    return 0;
}
