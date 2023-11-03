#ifndef RANDOM_MT_H
#define RANDOM_MT_H

#include <chrono>
#include <random>
#include "Constants.h"

// This header-only Random namespace implements a self-seeding Mersenne Twister
// It can be included into as many code files as needed (The inline keyword avoids ODR violations)
namespace Random
{
	inline std::mt19937 init()
	{
		std::random_device rd;

		// Create seed_seq with high-res clock and 7 random numbers from std::random_device
		std::seed_seq ss{
			static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()),
			rd(), rd(), rd(), rd(), rd(), rd(), rd() };

		return std::mt19937{ ss };
	}

	// Here's our std::mt19937 PRNG object
	// The inline keyword also means we only have one global instance for our whole program
	inline std::mt19937 mt{ init() };

	// Generate a random float_type between [min, max]
	inline float_type random_float(float_type min, float_type max)
	{
		std::uniform_real_distribution<float_type> distribution{ min, max };
		return distribution(mt); // and then generate a random number from our global generator
	}
};

#endif