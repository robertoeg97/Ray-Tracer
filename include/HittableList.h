#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
#include "Constants.h"
#include "Hittable.h"
#include "Ray3D.h"

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> hittables {};

    HittableList() = default;
    
    void clear() {hittables.clear();}

    void add(std::shared_ptr<Hittable> object) {
        hittables.push_back(object);
    }

    HitResult hit(const Ray3D& ray, float_type ray_tmin, float_type ray_tmax) const override {
        HitRecord hit_record {};
        bool hit_something = false;
        float_type t_closest = ray_tmax;
        for (const auto& hittable_ptr : hittables) {
            auto [is_closer_hit, cur_record] = hittable_ptr->hit(ray, ray_tmin, t_closest);
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