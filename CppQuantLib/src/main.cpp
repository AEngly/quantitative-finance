#include "CppQuantLib/BlackScholesFD.h"
#include <iostream>
#include <fstream>

int main() {

    // Get the parameters
    double spot = 1150;
    double strike = 1072;
    double rate = 0.05;
    double volatility = 0.2;
    double T = 5.00; // Maturity
    int M = 10000; // Discretization points in temporal dimension
    double max_N = std::sqrt((M / T) - rate) / volatility; // Use stability criterion
    int N = std::max(1, static_cast<int>(max_N * 0.99)); // Ensure N is at least 1

    // Print that we are computing the call option price
    std::cout << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Title: Call option pricing using the Black-Scholes PDE" << std::endl;
    std::cout << "Methodology: Finite difference" << std::endl;
    std::cout << std::endl;

    // Print the grid resolution
    std::cout << "Grid: [0, " << N << "]" << " x " << "[0, " << M << "]" << std::endl;
    std::cout << "Spatial range: [0, " << 2*spot << "]" << std::endl;
    std::cout << "Temporal range: [0, " << T << "]" << std::endl;

    // Solve the partial differential equation
    BlackScholesFD bsfd(spot, strike, rate, volatility, T, M, N);
    std::vector<std::vector<double>> priceGrid = bsfd.computePrices();
    std::vector<double> spatialGrid = bsfd.getSpatialGrid();
    std::vector<double> temporalGrid = bsfd.getTemporalGrid();

    // Print inputs for the user
    std::cout << std::endl;
    std::cout << "Spot: " << spot << std::endl;
    std::cout << "Strike: " << strike << std::endl;
    std::cout << "Rate: " << rate << std::endl;
    std::cout << "Volatility: " << volatility << std::endl;
    std::cout << "Maturity: " << T << std::endl;
    std::cout << std::endl;

    // Print the price of the option
    std::cout << "Call option price: " << priceGrid[N / 2][M] << std::endl;
    std::cout << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // Save price grid to a text file
    std::ofstream file;
    file.open("BlackScholesFD.txt");
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= M; ++j) {
            file << priceGrid[i][j] << ", ";
        }
        file << std::endl;
    }
    file.close();

    return 0;

}