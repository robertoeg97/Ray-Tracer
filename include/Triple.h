#ifndef TRIPLE_H
#define TRIPLE_H

#include <cmath>
#include <iostream>
#include <array>
#include "Constants.h"

template <typename Derived> 
class Triple {
protected:
    std::array<float_type, 3> m_vec {};

public:
    constexpr Triple() = default;
    constexpr Triple(float_type x, float_type y, float_type z) : m_vec {x, y, z} {}

    constexpr float_type x() const {return m_vec[0];}
    constexpr float_type y() const {return m_vec[1];}
    constexpr float_type z() const {return m_vec[2];}

    constexpr float_type operator[](int i) const {return m_vec[i];}
    constexpr float_type& operator[](int i) {return m_vec[i];}

    constexpr Derived operator-() const {return {-m_vec[0], -m_vec[1], -m_vec[2]};} 

    constexpr Derived operator+(const Derived& other) const {
        return {m_vec[0]+other.m_vec[0], m_vec[1]+other.m_vec[1], m_vec[2]+other.m_vec[2]};
    }
    constexpr Derived& operator+=(const Derived& other) {
        return *this = *this + other;
    }

    constexpr Derived operator-(const Derived& other) const {
        return {m_vec[0]-other.m_vec[0], m_vec[1]-other.m_vec[1], m_vec[2]-other.m_vec[2]};
    }
    constexpr Derived& operator-=(const Derived& other) {
        return *this = *this - other;
    }

    constexpr Derived operator*(float_type t) const {
        return {t*m_vec[0], t*m_vec[1], t*m_vec[2]};
    }
    constexpr Derived& operator*=(float_type t) {
        return *this = *this * t;
    }

    constexpr Derived operator/(float_type t) const {
        return *this * (1/t);
    }
    constexpr Derived& operator/=(float_type t) {
        return *this = *this / t;
    }

    float_type length() const {
        return std::sqrt(length_squared());
    }
    constexpr float_type length_squared() const {
        return m_vec[0]*m_vec[0] + m_vec[1]*m_vec[1] + m_vec[2]*m_vec[2];
    }

    constexpr float_type dot(const Derived& other) const {
        return m_vec[0] * other.m_vec[0] + m_vec[1] * other.m_vec[1] + m_vec[2] * other.m_vec[2];
    }

    constexpr Derived cross(const Derived& other) const {
        return {m_vec[1]*other.m_vec[2] - m_vec[2]*other.m_vec[1], 
                m_vec[2]*other.m_vec[0] - m_vec[0]*other.m_vec[2],
                m_vec[0]*other.m_vec[1] - m_vec[1]*other.m_vec[0]};
    }   

    Derived unit_vector() const {
        return *this / length();
    }

};

template <typename T>
constexpr inline T operator*(float_type t, const Triple<T>& trip) {
    const T& derived_trip = static_cast<const T&>(trip);   
    return derived_trip * t;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Triple<T>& trip) {
    const T& derived_trip = static_cast<const T&>(trip);   
    return std::cout << derived_trip.x() << ' ' << derived_trip.y() << ' ' << derived_trip.z();
}


#endif