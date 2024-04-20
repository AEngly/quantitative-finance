using System;
using MathNet.Numerics.Distributions;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Pricing;

class EuropeanOptionPricing
{
    static void Main()
    {

        // Parameters of the option
        double S = 90;  // Spot price
        double K = 95;  // Strike price
        double r = 0.05;  // Continuously compounded interest rate
        double sigma = 0.2;  // Volatility in years
        double T = 1;  // Time to maturity in years

        // Number of simulations
        ulong N = (ulong)Math.Pow(10, 9);
        Console.WriteLine($"\nMonte Carlo method using {N} simulations.");

        // Calculate option prices'
        var stopwatch = new Stopwatch();
        // Start timing
        stopwatch.Start();
        var (MCCall, MCPut) = MonteCarlo(S, K, r, sigma, T, N);
        // End timing
        stopwatch.Stop();
        var (BSCall, BSPut) = BlackScholes(S, K, r, sigma, T);
        Console.WriteLine($"\nEstimated call price: {MCCall}");
        Console.WriteLine($"Black-Scholes call price: {BSCall}\n");
        Console.WriteLine($"Estimated put price: {MCPut}");
        Console.WriteLine($"Black-Scholes put price: {BSPut}\n");
        Console.WriteLine($"Time elapsed: {stopwatch.ElapsedMilliseconds/1000.0} s\n");

        // Number of simulations
        ulong N2 = (ulong)Math.Pow(10, 7);
        Console.WriteLine($"\nMonte Carlo method parallelized using {N2} simulations.");

        // Calculate option prices'
        var stopwatch2 = new Stopwatch();
        // Start timing
        stopwatch2.Start();
        var (MCCall2, MCPut2) = MonteCarloParallelized(S, K, r, sigma, T, N2);
        // End timing
        stopwatch2.Stop();
        var (BSCall2, BSPut2) = BlackScholes(S, K, r, sigma, T);
        Console.WriteLine($"\nEstimated call price: {MCCall2}");
        Console.WriteLine($"Black-Scholes call price: {BSCall2}\n");
        Console.WriteLine($"Estimated put price: {MCPut2}");
        Console.WriteLine($"Black-Scholes put price: {BSPut2}\n");
        Console.WriteLine($"Time elapsed: {stopwatch2.ElapsedMilliseconds/1000.0} s\n");

    }

    static double SampleNormal(Random rand, double mean, double stddev)
    {
        // Generate a uniform random number
        double uniform = rand.NextDouble();

        // Use the inverse CDF to get a normal distributed random number
        return Normal.InvCDF(mean, stddev, uniform);
    }

    // Calculate result with Black-Scholes
    static (double, double) BlackScholes(double S, double K, double r, double sigma, double T)
    {
        double d1 = (Math.Log(S / K) + (r + sigma * sigma / 2) * T) / (sigma * Math.Sqrt(T));
        double d2 = d1 - sigma * Math.Sqrt(T);
        double callPrice = S * Normal.CDF(0,1,d1) - K * Math.Exp(-r * T) * Normal.CDF(0,1,d2);
        double putPrice = K * Math.Exp(-r * T) * Normal.CDF(0,1,-d2) - S * Normal.CDF(0,1,-d1);

        // return both call and put as a tuple
        return (callPrice, putPrice);
    }

    static (double, double) MonteCarlo(double S, double K, double r, double sigma, double T, ulong N)
    {
        double expectationCall = 0;
        double expectationPut = 0;
        Random rand = new Random();
        for (ulong i = 0; i < N; i++)
        {
            double Z = SampleNormal(rand, 0, 1);
            double ST = S * Math.Exp((r - sigma * sigma / 2) * T + sigma * Math.Sqrt(T) * Z);
            expectationCall += Math.Max(ST - K, 0)/N;
            expectationPut += Math.Max(K - ST, 0)/N;
        }
        return (Math.Exp(-r * T)*expectationCall, Math.Exp(-r * T)*expectationPut);
    }

    static (double, double) MonteCarloParallelized(double S, double K, double r, double sigma, double T, ulong N)
    {
        double totalExpectationCall = 0;
        double totalExpectationPut = 0;
        object lockObj = new object();

        Parallel.For(0, (long)N,
            // Local state initialization
            () => new { Rand = new Random(Guid.NewGuid().GetHashCode()), Call = 0.0, Put = 0.0 },
            (i, loopState, localState) =>
            {
                double Z = SampleNormal(localState.Rand, 0, 1);
                double ST = S * Math.Exp((r - sigma * sigma / 2) * T + sigma * Math.Sqrt(T) * Z);
                double callPayoff = Math.Max(ST - K, 0) / N;
                double putPayoff = Math.Max(K - ST, 0) / N;

                // Accumulate results in local state
                return new { localState.Rand, Call = localState.Call + callPayoff, Put = localState.Put + putPayoff };
            },
            // Final action for each local state
            localState =>
            {
                lock (lockObj)
                {
                    totalExpectationCall += localState.Call;
                    totalExpectationPut += localState.Put;
                }
            });

        return (Math.Exp(-r * T) * totalExpectationCall, Math.Exp(-r * T) * totalExpectationPut);
    }


}