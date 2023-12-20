#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <cmath>
#include <random>
#include <assert.h>
#include <functional>

#define INVALID_ID 0

namespace lazyDungeon{
using uuid64 = size_t;
uuid64 generate_uuid();
std::string n_to_string( float x );
int getRandomNumber(int min, int max);
}


#endif // PRECOMPILED_H
