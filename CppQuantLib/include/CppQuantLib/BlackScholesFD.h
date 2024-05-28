#ifndef BLACKSCHOLESFD_H
#define BLACKSCHOLESFD_H

#include <vector>

class BlackScholesFD {
public:
    BlackScholesFD(double spot, double strike, double rate, double volatility, double T, int M, int N);
    std::vector<std::vector<double>> computePrices();
    std::vector<double> getSpatialGrid();
    std::vector<double> getTemporalGrid();

private:
    double spot_;
    double strike_;
    double rate_;
    double volatility_;
    double time_;
    int M_;
    int N_;
    double T_;
    double spatialMax_;
    double dS;
    double dt;
    std::vector<double> spatialGrid_;
    std::vector<double> temporalGrid_;
    std::vector<std::vector<double>> optionPrices_;

    void initializeGrid();
    void initializeOptionPrices();
    void applyBoundaryConditions();
    void explicitPass();
};

#endif // BLACKSCHOLESFD_H