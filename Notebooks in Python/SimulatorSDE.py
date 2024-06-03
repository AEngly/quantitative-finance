# ------------------ PACKAGES ------------------

import numpy as np
import math
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.stats import norm
import time
import numpy as np
import math
from scipy.stats import norm
from tqdm import tqdm

# ------------------ EULER-MARUYAMA IMPLEMENTATION ------------------

def SDE_simulation(tN = 100, t0 = 0, f = lambda X_t, t : 0, g = lambda X_t, t : 1, delta_t = 0.001, X_0 = 0, n_sim = 10, plot = False, title = 'Cox-Ingersoll-Ross', verbose = False):

    start = time.time()

    size = math.ceil((tN - t0)/delta_t)
    time_points = [t0] + [delta_t for i in range(0, size)]

    column_names = ['Time'] + ['Simulation {}'.format(i) for i in range(1, n_sim+1)];
    time_array = np.cumsum(time_points);

    # Simulation (for now equal time steps are assumed)
    dWt = norm.ppf(np.random.rand(size + 1,n_sim), loc = 0, scale = math.sqrt(delta_t))

    # Euler Maruyama
    simulation_array = np.zeros((size + 1,n_sim))
    simulation_array[0,:]  = X_0;
    for i in tqdm (range(0, len(time_points)-1), desc="Numerical Integration", ascii=False, ncols=75):
        simulation_array[i+1,:] = simulation_array[i,:] + f(simulation_array[i,:], time_array[i]) * time_points[i+1] + g(simulation_array[i,:], time_array[i]) * dWt[i]

    # Then we can make a Pandas data frame
    df = pd.DataFrame(np.column_stack([time_array, simulation_array]));
    df.columns = column_names;

    end = time.time()

    if verbose:
        print("\nTime to run simulations: {}s \n".format(end - start))
        print("The output has {} rows and {} columns.".format(df.shape[0], df.shape[1]))
        print("The total number of elements is {}.\n".format(df.shape[0]*df.shape[1]))

    if plot:

        plt.rcParams.update({
            "text.usetex": True,
            "font.family": "Helvetica"
        })

        print("Plotting has started ...\n")
        plt.figure(figsize=(10,6), dpi = 100)
        plt.xlabel('t', fontsize = 14)
        plt.ylabel(r'$X_{t}$', fontsize = 14)
        plt.title(title, fontsize = 18)
        plt.plot(df.iloc[:,0].values, df.iloc[:,1:].values);
        plt.show()

    return(df)

# ------------------ VARIOUS MODELS ------------------

# 1. Building block

def StandardBrownianMotion(tN = 100, t0 = 0, delta_t = 0.001, B_0 = 0, n_sim = 10, plot = False, title = r'\textbf{Standard Brownian Motion (i.e., $\{B_{t}\}_{t \geq t_{0}}$)}'):
    return SDE_simulation(tN = tN, t0 = t0, delta_t = 0.001, X_0 = B_0, n_sim = n_sim, plot = plot, title = title)

# 2. Simulation of asset prices

def GeometricBrownianMotion(r = 0.1, sigma = 0.2, tN = 100, t0 = 0, delta_t = 0.001, X_0 = 0, n_sim = 10, plot = False, title = r'Geometric Brownian Motion'):

    def f(state: float, t: float)->"Drift":
        return(state*r)

    def g(state: float, t: float)->"Diffusion":
        return(state*sigma)

    return SDE_simulation(tN = tN, f = f, g = g, t0 = t0, delta_t = delta_t, X_0 = X_0, n_sim = n_sim, plot = plot, title = title)

# 3. Simulation of interest rates

def CoxIngersollRoss(lambdA = 0.1, xi = 0.2, gamma = 0.3, tN = 100, t0 = 0, delta_t = 0.001, B_0 = 0, n_sim = 10, plot = False, title = r'Cox-Ingersoll-Ross'):

    def f(state: float, t: float)->"Drift":
        return(lambdA * (xi - state))

    def g(state: float, t: float)->"Diffusion":
        return(gamma * np.sqrt(state))

    return SDE_simulation(tN = tN, f = f, g = g, t0 = t0, delta_t = 0.001, X_0 = B_0, n_sim = n_sim, plot = plot, title = title)

def Vasicek(k = 0.1, theta = 0.2, sigma = 0.2, tN = 100, t0 = 0, delta_t = 0.001, X_0 = 0, n_sim = 10, plot = False, title = r'Vasicek'):

    def f(state: float, t: float)->"Drift":
        return(k * (theta - state))

    def g(state: float, t: float)->"Diffusion":
        return(sigma)

    return SDE_simulation(tN = tN, f = f, g = g, t0 = t0, delta_t = delta_t, X_0 = X_0, n_sim = n_sim, plot = plot, title = title)

# 4. Bond Prices under Various Models

def B (t, T, k):
    return((1 - np.exp(-k*(T-t)))/k)

def A(t, T, k, theta, sigma):
    return(np.exp((theta - (sigma**2)/(2*(k**2))) * (B(t, T, k) - T + t) - ((sigma**2)/(4*k))*(B(t,T,k)**2)))

def P(t, T, r, k, theta, sigma):
    return(A(t,T,k,theta,sigma)*np.exp(-r*B(t,T,k)))

def simulateBondPrices(k = 0.1, theta = 0.2, sigma = 0.2, T = 100, t0 = 0, delta_t = 0.001, X_0 = 0, n_sim = 10, plot = False, title = r'Bond Prices (Vasicek)'):

    # apply function P to every simulation element in Vasicek

    spotRates = Vasicek(k = k, theta = theta, sigma = sigma, tN = T, t0 = t0, delta_t = delta_t, X_0 = X_0, n_sim = n_sim, plot = False, title = title)
    bondPrices = spotRates.copy()

    for index,t in enumerate(bondPrices.iloc[:,0]):
        for sim in range(1, bondPrices.shape[1]):
            bondPrices.iloc[index, sim] = P(t, T, bondPrices.iloc[index, sim], k, theta, sigma)

    if plot:

        plt.rcParams.update({
            "text.usetex": True,
            "font.family": "Helvetica"
        })

        print("Plotting has started ...\n")
        plt.figure(figsize=(10,6), dpi = 100)
        plt.xlabel('t', fontsize = 14)
        plt.ylabel(r'$ZCB(t, {})$'.format(T), fontsize = 14)
        plt.title(title, fontsize = 18)
        plt.plot(bondPrices.iloc[:,0].values, bondPrices.iloc[:,1:].values)

    return(bondPrices, spotRates)