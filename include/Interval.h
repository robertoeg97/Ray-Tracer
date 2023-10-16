#ifndef INTERVAL_H
#define INTERVAL_H

#include "Constants.h"

class Interval {
public:
    static const Interval empty;
    static const Interval universe;
    float_type min {+infinity};  //default is empty
    float_type max {-infinity};  //default is empty
    constexpr Interval() = default; 
    constexpr Interval(float_type min_, float_type max_) : min{min_}, max{max_} {}

    bool contains(float_type x) const {
        return x >= min && x <= max;
    }

    float_type clamp(float_type x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
};

const Interval Interval::empty {+infinity, -infinity};
const Interval Interval::universe {-infinity, +infinity};

#endif