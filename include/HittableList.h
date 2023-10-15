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

    HitResult hit(const Ray3D& ray, Interval t_interval) const override {
        HitRecord hit_record {};
        bool hit_something = false;
        float_type t_closest = t_interval.max;
        for (const auto& hittable_ptr : hittables) {
            auto [is_closer_hit, cur_record] = hittable_ptr->hit(ray, Interval{t_interval.min, t_closest});
            if (is_closer_hit) {
                hit_something = true;
                t_closest = cur_record.t;
                hit_record = cur_record;
            }
        }

        return {hit_something, hit_record};
    }
};


#endif