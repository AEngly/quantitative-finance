#include "CppQuantLib/BlackScholesFD.h"
#include <cmath>
#include <algorithm>
#include <iostream>

BlackScholesFD::BlackScholesFD(double spot, double strike, double rate, double volatility, double time, int steps, int gridPoints)
    : spot_(spot), strike_(strike), rate_(rate), volatility_(volatility), time_(time), steps_(steps), gridPoints_(gridPoints), grid_(gridPoints), optionPrices_(gridPoints, std::vector<double>(steps + 1)) {
    initializeGrid();
    initializeOptionPrices();
}

void BlackScholesFD::initializeGrid() {
    double dS = 2.0 * spot_ / (gridPoints_ - 1);
    for (int i = 0; i < gridPoints_; ++i) {
        grid_[i] = i * dS;
    }
}

void BlackScholesFD::initializeOptionPrices() {
    for (int i = 0; i < gridPoints_; ++i) {
        optionPrices_[i][0] = std::max(grid_[i] - strike_, 0.0); // Initial condition C_{i,0}
    }
    applyBoundaryConditions();
}

void BlackScholesFD::applyBoundaryConditions() {
    for (int j = 0; j <= steps_; ++j) {
        optionPrices_[0][j] = 0; // Boundary condition C_{0,j}
        optionPrices_[gridPoints_ - 1][j] = grid_.back(); // Boundary condition C_{N,j}
    }
}

void BlackScholesFD::stepForward() {

    // Define local variables
    double dt = time_ / steps_;
    double dx = 2.0 * spot_ / (gridPoints_ - 1);
    double alpha = dt * volatility_ * volatility_ / (dx * dx);
    double beta = dt * rate_ / dx;

    // Step forward in time
    for (int j = 0; j < steps_; ++j) {
        for (int i = 1; i < gridPoints_ - 1; ++i) {
            optionPrices_[i][j + 1] = (1 - beta - i * i * alpha) * optionPrices_[i][j] +
                                      0.5 * (i * i * alpha + i * beta) * optionPrices_[i + 1][j] -
                                      0.5 * (i * i * alpha - i * beta) * optionPrices_[i - 1][j];
        }
    }
}

double BlackScholesFD::price() {
    stepForward();
    return optionPrices_[gridPoints_ / 2][steps_];
}