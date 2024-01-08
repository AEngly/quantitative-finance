using System;
using MathNet.Numerics.Distributions;

namespace Pricing;

class EuropeanOptionPricing
{
    static void Main(String[] args)
    {

        // Printing first arg
        Console.WriteLine($"\nTask: {args[0]}\n");

        // Parameters of the option
        double S = 100;  // Spot price
        double K = 110;  // Strike price
        double r = 0.05;  // Continuously compounded interest rate
        double sigma = 0.2;  // Volatility in years
        double T = 1;  // Time to maturity in years

        // Number of simulations
        int N = 100000000;

        // Calculate option prices
        double riskNeutralPriceCall = MonteCarlo(S, K, r, sigma, T, N, true);
        Console.WriteLine($"\nEstimated call price: {riskNeutralPriceCall}");
        double BSPriceCall = BlackScholesCall(S, K, r, sigma, T);
        Console.WriteLine($"Black-Scholes call price: {BSPriceCall}\n");
        double riskNeutralPricePut = MonteCarlo(S, K, r, sigma, T, N, false);
        Console.WriteLine($"\nEstimated call price: {riskNeutralPricePut}");
        double BSPricePut = BlackScholesPut(S, K, r, sigma, T);
        Console.WriteLine($"Black-Scholes call price: {BSPricePut}\n");

    }

    static double SampleNormal(Random rand, double mean, double stddev)
    {
        // Generate a uniform random number
        double uniform = rand.NextDouble();

        // Use the inverse CDF to get a normal distributed random number
        return Normal.InvCDF(mean, stddev, uniform);
    }

    // Calculate result with Black-Scholes
    static double BlackScholesCall(double S, double K, double r, double sigma, double T)
    {
        double d1 = (Math.Log(S / K) + (r + sigma * sigma / 2) * T) / (sigma * Math.Sqrt(T));
        double d2 = d1 - sigma * Math.Sqrt(T);
        double callPrice = S * Normal.CDF(0,1,d1) - K * Math.Exp(-r * T) * Normal.CDF(0,1,d2);
        return callPrice;
    }

    static double BlackScholesPut(double S, double K, double r, double sigma, double T)
    {
        double d1 = (Math.Log(S / K) + (r + sigma * sigma / 2) * T) / (sigma * Math.Sqrt(T));
        double d2 = d1 - sigma * Math.Sqrt(T);
        double putPrice = K * Math.Exp(-r * T) * Normal.CDF(0,1,-d2) - S * Normal.CDF(0,1,-d1);
        return putPrice;
    }

    static double MonteCarlo(double S, double K, double r, double sigma, double T, int N, bool isCall)
    {
        Random rand = new Random();
        double sum = 0;
        for (int i = 0; i < N; i++)
        {
            double Z = SampleNormal(rand, 0, 1);
            double ST = S * Math.Exp((r - sigma * sigma / 2) * T + sigma * Math.Sqrt(T) * Z);
            if (isCall)
                sum += Math.Max(ST - K, 0);
            else
                sum += Math.Max(K - ST, 0);
        }
        return Math.Exp(-r * T) * sum / N;
    }


}