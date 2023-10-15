#ifndef INTERVAL_H
#define INTERVAL_H

#include "Constants.h"

class Interval {
public:
    static constexpr Interval empty;
    static constexpr Interval universe;
    float_type min {+infinity};  //default is empty
    float_type max {-infinity};  //default is empty
    constexpr Interval() = default; 
    constexpr Interval(float_type min_, float_type max_) : min{min_}, max{max_} {}

    bool contains(float_type x) {
        return x >= min && x <= max;
    }

    bool surrounds(float_type x) const {
        return min < x && x < max;
    }
};

constexpr Interval Interval::empty {+infinity, -infinity};
constexpr Interval Interval::universe {-infinity, +infinity};

#endif