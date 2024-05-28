#ifndef BLACKSCHOLESFD_H
#define BLACKSCHOLESFD_H

#include <vector>

class BlackScholesFD {
public:
    BlackScholesFD(double spot, double strike, double rate, double volatility, double time, int steps, int gridPoints);
    double price();

private:
    double spot_;
    double strike_;
    double rate_;
    double volatility_;
    double time_;
    int steps_;
    int gridPoints_;
    std::vector<double> grid_;
    std::vector<std::vector<double>> optionPrices_;

    void initializeGrid();
    void initializeOptionPrices();
    void applyBoundaryConditions();
    void stepForward();
};

#endif // BLACKSCHOLESFD_H