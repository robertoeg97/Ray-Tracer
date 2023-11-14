#ifndef BVH_H
#define BVH_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Hittable.h"
#include "HittableList.h"
#include "AABB.h"
#include "Random.h"

class BVH_node : public Hittable {
public:
    BVH_node(std::vector<std::shared_ptr<Hittable>> hittable_vec) {
        //choose random axis to use for hittable group segregation
        int axis_i = Random::random_int(0, 2);
        auto comparator = (axis_i == 0) ? box_x_compare
                        : (axis_i == 1) ? box_y_compare
                                        : box_z_compare;
        //sort the elements so that they can be divided into two non-overloapping bounding boxes
        std::sort(hittable_vec.begin(), hittable_vec.end(), comparator);

        auto num_elements = hittable_vec.size();
        //base case 1
        if (num_elements == 1) {
            left = right = hittable_vec[0];
        }
        //base case 2
        else if (num_elements == 2) {
            left = hittable_vec[0];
            right = hittable_vec[1];
        }
        //recurse
        else {
            auto mid = num_elements/2;
            left = std::make_shared<BVH_node>(std::vector<std::shared_ptr<Hittable>>(hittable_vec.begin(), hittable_vec.begin()+mid));
            right = std::make_shared<BVH_node>(std::vector<std::shared_ptr<Hittable>>(hittable_vec.begin()+mid, hittable_vec.end()));
        }

        bbox = AABB{left->bounding_box(), right->bounding_box()};
    }

    BVH_node(const HittableList& hittable_list) : BVH_node(hittable_list.hittables) {}

    HitRecord hit(const Ray3D& ray, const Interval& t_interval) const override {
        if (!bbox.hit(ray, t_interval)) {
            return HitRecord{false};
        }

        HitRecord left_hit = left->hit(ray, t_interval);
        HitRecord right_hit = right->hit(ray, Interval{t_interval.min, left_hit.is_hit ? left_hit.t : t_interval.max});

        if (right_hit.is_hit) { //closer possible hit first
            return right_hit;
        }
        else if (left_hit.is_hit) { 
            return left_hit;
        }
        else {
            return HitRecord{false};
        }
    }

    AABB bounding_box() const override {
        return bbox;
    }

private:
    static bool box_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2, int axis_i) {
        return h1->bounding_box().axis(axis_i).min < h2->bounding_box().axis(axis_i).min;
    }

    static bool box_x_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 0);
    }

    static bool box_y_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 1);
    }

    static bool box_z_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 2);
    }

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;
};

#endif