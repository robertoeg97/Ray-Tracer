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
     * @brief Constructs a new Interval object that merges two existing Intervals.
     * Any value that was a member of either of the two Intervals will be a part of the returned Interval.
     * 
     */
    constexpr Interval(const Interval& first, const Interval& second) :
        min (fmin(first.min, second.min)), 
        max (fmax(first.max, second.max))
    {}

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

    /**
     * @brief returns the size of the Interval
     * 
     * @return float_type the difference between the max and min Interval values
     */
    float_type size() const {
        return max - min;
    }

    /**
     * @brief returns a new Interval that expands the old Interval outwards by some amount
     * 
     * @param delta the value to expand both ends of the Interval by
     * @return Interval the expanded Interval
     */
    Interval expand(float_type delta) const {
        float_type padding = delta / 2;
        return Interval {min - padding, max + padding};
    }
};

const Interval Interval::empty {+infinity, -infinity};
const Interval Interval::universe {-infinity, +infinity};

/**
 * @brief Shifts both ends of an Interval by some amount.
 * 
 * @param interval the Interval to move
 * @param displacement the amount to shift the Interval by
 * @return Interval the resulting displaced Interval
 */
Interval operator+(const Interval& interval, float_type displacement) {
    return Interval{interval.min + displacement, interval.max+displacement};
}

/**
 * @brief Shifts both ends of an Interval by some amount.
 * 
 * @param displacement the amount to shift the Interval by
 * @param interval the Interval to move
 * @return Interval the resulting displaced Interval
 */
Interval operator+(float_type displacement, const Interval& interval) {
    return Interval{interval.min + displacement, interval.max+displacement};
}

#endif