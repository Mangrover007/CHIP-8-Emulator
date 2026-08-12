/*
 * In C++, the <random> library is used to generate random numbers.
 * But how does a random number get generated?
 *
 * Machines generate pseudo random numbers. The simplest formula
 * that generates a sequence of psuedo random numbers is the
 * Linear Congruential Generator (LCG) and it is:
 *
 * X_{n+1} = (A \cdot X_n + c) mod m
 *
 * There are other formulae that are more convoluted but all of
 * these require a starting value, called the SEED. The function
 * itself is called the "random engine". We need the SEED to drive
 * that engine. The SEED is called the "random device".
 *
 * The SEED can be the system clock for example. Another random
 * device is /dev/random.
 *
 * Then, we need one last component. We want the random numbers
 * to follow a fixed distribution. All in all:
 *
 * - random device
 * - random engine
 * - distribution
 *
 * in C++, we will use the std::uniform_int_distribution<> for the
 * distribution. We will need to feed this thing the random engine
 * every time we want a random number, like so:
 *
 * std::mt19973 random_engine(<seed>);
 * std::uniform_int_distribution<int> rng(<low range>, <high range>);
 *
 * rng(random_engine); --> this produces a random number between
 * <low range> and <high range>
 *
 * There is the default_random_engine as well.
 *
 * std::default_random_engine randEngine(<seed>);
 * std::uniform_int_distribution<int> rng(0, 255);
 *
 * rng(randEngine); --> this produces a random number between 0 and 255
 * from the default_random_engine formula. This formula is:
 *
 * */

#include <random>
#include <iostream>
#include <chrono>
#include <ctime>

int main()
{
    auto p0 = std::chrono::time_point<std::chrono::system_clock>{};
    auto p1 = std::chrono::system_clock::now();

    std::cout << std::chrono::system_clock::to_time_t(p1) << '\n';
    std::time_t epoch = std::chrono::system_clock::to_time_t(p1);
    std::cout << std::ctime(&epoch);

    std::default_random_engine randEngine(std::chrono::system_clock::to_time_t(p1));
    std::uniform_int_distribution<uint> rng(0, 255U);

    for (int i=0; i<5; i++)
    {
	std::cout << rng(randEngine) << '\n';
    }

    return 0;
}

