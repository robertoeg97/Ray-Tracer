#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
#include "Constants.h"
#include "Hittable.h"
#include "Ray3D.h"
#include "Interval.h"

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> hittables {};

    HittableList() = default;
    
    void clear() {hittables.clear();}

    void add(std::shared_ptr<Hittable> object) {
        hittables.push_back(object);
    }

    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        HitRecord hit_record {};
        float_type t_closest = t_interval.max;
        for (const auto& hittable_ptr : hittables) {
            HitRecord cur_record = hittable_ptr->hit(ray, Interval{t_interval.min, t_closest});
            if (cur_record.is_hit) {
                t_closest = cur_record.t;
                hit_record = cur_record;
            }
        }

        return hit_record;
    }
};


#endif