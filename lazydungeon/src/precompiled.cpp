#include "precompiled.h"

namespace lazyDungeon {

static std::random_device randomizer;
static std::mt19937_64 generator(randomizer());
static std::uniform_int_distribution<uuid64> distribution;

uuid64 generate_uuid() {
    uuid64 uuid = INVALID_ID;
    do {
        uuid = distribution(generator);
    } while (uuid == INVALID_ID);
    return uuid;
}

std::string n_to_string( float x ) {
    int length = snprintf( NULL, 0, "%2.f", x );
    assert( length >= 0 );
    char* buf = new char[length + 1];
    snprintf( buf, length + 4, "%.2f", x );
    std::string str( buf );
    delete[] buf;
    return str;
}

int getRandomNumber(int min, int max) {
    std::random_device rd;  // Seed for the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister 19937 generator

    // Define a distribution for the desired range [min, max]
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate and return a random number
    return distribution(gen);
}
}
