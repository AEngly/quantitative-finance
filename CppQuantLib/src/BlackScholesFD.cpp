#include "CppQuantLib/BlackScholesFD.h"
#include <cmath>
#include <algorithm>
#include <iostream>

BlackScholesFD::BlackScholesFD(double spot, double strike, double rate, double volatility, double T, int M, int N) {

    // Intialize member variables
    spot_ = spot;
    strike_ = strike;
    rate_ = rate;
    volatility_ = volatility;
    M_ = M;
    N_ = N;
    T_ = T;
    spatialMax_ = 2.0 * spot;
    dS = spatialMax_ / N_;
    dt = T_ / M_;
    spatialGrid_ = std::vector<double>(N_ + 1);
    temporalGrid_ = std::vector<double>(M_ + 1);
    optionPrices_ = std::vector<std::vector<double>>(N_ + 1, std::vector<double>(M_ + 1));

    // Make sure optionPrices_ is initialized to zero
    for (int i = 0; i <= N_; ++i) {
        for (int j = 0; j <= M_; ++j) {
            optionPrices_[i][j] = 0;
        }
    }

    // Run private methods
    initializeGrid();
    initializeOptionPrices();
}

void BlackScholesFD::initializeGrid() {
    for (int i = 0; i <= N_; ++i) {
        spatialGrid_[i] = i * dS;
    }
    for (int i = 0; i <= M_; ++i) {
        temporalGrid_[i] = i * dt;
    }
}

void BlackScholesFD::initializeOptionPrices() {
    for (int i = 0; i <= N_; ++i) {
        optionPrices_[i][0] = std::max(spatialGrid_[i] - strike_, 0.0); // Initial condition C_{i,0}
    }
    applyBoundaryConditions();
}

void BlackScholesFD::applyBoundaryConditions() {
    for (int j = 0; j <= M_; ++j) {
        optionPrices_[0][j] = 0; // Boundary condition C_{0,j}
        optionPrices_[N_][j] = dS * N_ - strike_*exp(-rate_ * (j * dt)); // Boundary condition C_{N,j}
    }
}

void BlackScholesFD::explicitPass() {

    // Define local variables
    double alpha = dt * volatility_ * volatility_;
    double beta = dt * rate_;

    // Step forward in time
    for (int j = 0; j <= M_ - 1; ++j) {
        for (int i = 1; i <= N_ - 1; ++i) {
            optionPrices_[i][j + 1] = (1 - beta - i * i * alpha) * optionPrices_[i][j] + 0.5 * (i * i * alpha + i * beta) * optionPrices_[i + 1][j] + 0.5 * (i * i * alpha - i * beta) * optionPrices_[i - 1][j];
        }
    }
}

std::vector<double> BlackScholesFD::getSpatialGrid() {
    return spatialGrid_;
}

std::vector<double> BlackScholesFD::getTemporalGrid() {
    return temporalGrid_;
}

std::vector<std::vector<double>> BlackScholesFD::computePrices() {
    explicitPass();
    return optionPrices_;
}