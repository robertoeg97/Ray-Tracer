#ifndef INTERVAL_H
#define INTERVAL_H

#include "Constants.h"

/**
 * @brief Class containing information about an internval from min to max
 * 
 */
class Interval {
public:
    static const Interval empty;
    static const Interval universe;
    float_type min {+infinity};  //default is empty
    float_type max {-infinity};  //default is empty

    constexpr Interval() = default; 

    /**
     * @brief Construct a new Interval object
     * 
     * @param min_ the lower bound of the interval
     * @param max_ the upper bound of the interval
     */
    constexpr Interval(float_type min_, float_type max_) : min{min_}, max{max_} {}

    /**
     * @brief Returns whether a value is contained within the interval (inclusive).
     * 
     * @param x the value in question
     * @return true if the value is inside of the interval
     * @return false otherwise
     */
    bool contains(float_type x) const {
        return x >= min && x <= max;
    }

    /**
     * @brief Returns whether a value is contained within the interval (exclusive).
     * 
     * @param x the value in question
     * @return true if the value is inside of the interval
     * @return false otherwise
     */
    bool surrounds(float_type x) const {
        return x > min && x < max; 
    }

    /**
     * @brief Returns the closest value to x that is contained within the interval (inclusive).
     * 
     * @param x the value in question
     * @return float_type the value closest to x that is inside of the interval
     */
    float_type clamp(float_type x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
};

const Interval Interval::empty {+infinity, -infinity};
const Interval Interval::universe {-infinity, +infinity};

#endif