#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
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

    bool hit(const Ray3D& ray, float_type ray_tmin, float_type ray_tmax, HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_something = false;
        float_type t_closest = ray_tmax;
        for (const auto& hittable_ptr : hittables) {
            if (hittable_ptr->hit(ray, ray_tmin, t_closest, temp_rec)) {
                hit_something = true;
                t_closest = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_something;
    }
};


#endif