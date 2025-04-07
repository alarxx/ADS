#include <iostream>

#include "random.hpp"

// https://en.cppreference.com/w/cpp/numeric/random
// https://en.cppreference.com/w/cpp/numeric/random/uniform_random_bit_generator
int main() {
    Random<long> rand(0, 99);
    std::cout << "min: " << rand.min() << ", max: " << rand.max() << std::endl;
    std::cout << rand() << std::endl;
}

/*

g++ main.cpp -Wall -std=c++23 -o app.out && ./app.out

*/
