#ifndef BVH_H
#define BVH_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Hittable.h"
#include "HittableList.h"
#include "AABB.h"
#include "Random.h"

/**
 * @brief A Bounding Volume Heirarchy class, which serves as an alternative to the HittableList.
 * Primarily used to compute the first Hittable in a HittableList that collides with a ray,
 * more quickly than the linear scan of a HittableList.
 * Uses a binary tree to find the first object hit in logarithmic time.
 */
class BVH_node : public Hittable {
public:
    /**
     * @brief Construct a new bvh binary tree node object out of a given vector of Hittable objects.
     * 
     * @param hittable_vec the hittable vector
     */
    BVH_node(std::vector<std::shared_ptr<Hittable>> hittable_vec) {
        //choose random axis to use for hittable group segregation
        int axis_i = Random::random_int(0, 2);
        auto comparator = (axis_i == 0) ? box_x_compare
                        : (axis_i == 1) ? box_y_compare
                                        : box_z_compare;
        //sort the elements so that they can be divided into two non-overloapping bounding boxes
        std::sort(hittable_vec.begin(), hittable_vec.end(), comparator);

        auto num_elements = static_cast<long>(hittable_vec.size());
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

    /**
     * @brief Construct a new bvh node object out of a HittableList.
     * This constructor will return the root node of the BVH.
     * 
     * @param hittable_list the Hittable LIst object.
     */
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

    /**
     * @brief returns the bounding box that contains all hittable objects contained in this node.
     * 
     * @return AABB the bounding box
     */
    AABB bounding_box() const override {
        return bbox;
    }

private:
    /**
     * @brief Returns true if the bounding box of a hittable h1 has a lower min coordinate than h2
     * for the given axis (indexed from 0-2).
     * For two disjoint boxes, this guarantees that all of the axis coordinates of h1 are < those of h2.
     * 
     * @param h1 the first hittable
     * @param h2 the second hittable
     * @param axis_i the index of the axis to compare coordinates of
     * @return true if the first hittable box has a lower min coordinate than the second
     * @return false otherwise
     */
    static bool box_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2, int axis_i) {
        return h1->bounding_box().axis(axis_i).min < h2->bounding_box().axis(axis_i).min;
    }

    /**
     * @brief returns whether one hittable's bounding box has a smaller x-coordinate than the other.
     * 
     * @param h1 the first hittable
     * @param h2 the second hittable
     * @return true if h1's bounding box has a lower x-coordinate than h2's.
     * @return false otherwise
     */
    static bool box_x_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 0);
    }

    /**
     * @brief returns whether one hittable's bounding box has a smaller y-coordinate than the other.
     * 
     * @param h1 the first hittable
     * @param h2 the second hittable
     * @return true if h1's bounding box has a lower y-coordinate than h2's.
     * @return false otherwise
     */
    static bool box_y_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 1);
    }

    /**
     * @brief returns whether one hittable's bounding box has a smaller z-coordinate than the other.
     * 
     * @param h1 the first hittable
     * @param h2 the second hittable
     * @return true if h1's bounding box has a lower z-coordinate than h2's.
     * @return false otherwise
     */
    static bool box_z_compare(const std::shared_ptr<Hittable>& h1, const std::shared_ptr<Hittable>& h2) {
        return box_compare(h1, h2, 2);
    }

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;
};

#endif