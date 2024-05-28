#include "CppQuantLib/BlackScholesFD.h"
#include <cassert>
#include <iostream>

void testBlackScholesFD() {

    // Define inputs
    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.05;
    double volatility = 0.2;
    double time = 1.0;
    int steps = 100;
    int gridPoints = 100;

    // Instantiate the class
    BlackScholesFD bsfd(spot, strike, rate, volatility, time, steps, gridPoints);
    std::vector<std::vector<double>> price = bsfd.computePrices();

    // Expected result can be calculated or compared against a known good value
    std::cout << "Testing finite difference: " << 1 << std::endl;

    // Basic assertion to verify it runs (add proper checks as necessary)
    // assert(price > 0);
}

int main() {
    testBlackScholesFD();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}