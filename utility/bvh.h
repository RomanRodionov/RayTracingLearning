#ifndef BVH_H
#define BVH_H

#include "common.h"
#include "object.h"
#include "objects_list.h"
#include "aabb.h"

class bvh_node : public Object
{
    private:
        shared_ptr<Object> left;
        shared_ptr<Object> right;
        aabb bbox;

        static bool box_compare(const shared_ptr<Object> a, const shared_ptr<Object> b, int axis)
        {
            return a->bounding_box().axis(axis).min < b->bounding_box().axis(axis).min;
        }
        static bool box_x_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
        {
            return box_compare(a, b, 0);
        }
        static bool box_y_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
        {
            return box_compare(a, b, 1);
        }
        static bool box_z_compare(const shared_ptr<Object> a, const shared_ptr<Object> b)
        {
            return box_compare(a, b, 2);
        }
    public:
        bvh_node(const ObjectsList& list) : bvh_node(list.objects, 0, list.objects.size()) {}
        bvh_node(const std::vector<shared_ptr<Object>>& src_objects, size_t start, size_t end) 
        {
            std::vector<shared_ptr<Object>> objects = src_objects;

            int axis = rand_int(0, 2);
            auto comparator = (axis == 0) ? box_x_compare
                            : (axis == 1) ? box_y_compare
                                          : box_z_compare;

            size_t object_span = end - start;

            if (object_span == 1)
            {
                left = right = objects[start];
            } 
            else if (object_span == 2)
            {
                if (comparator(objects[start], objects[start + 1]))
                {
                    left = objects[start];
                    right = objects[start + 1];
                } 
                else
                {
                    left = objects[start + 1];
                    right = objects[start];
                }
            } 
            else
            {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);
                int mid = start + object_span / 2;
                left = make_shared<bvh_node>(objects, start, mid);
                right = make_shared<bvh_node>(objects, mid, end);
            }

            bbox = aabb(left->bounding_box(), right->bounding_box());
        }
        bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override
        {
            if (!bbox.hit(ray, ray_t)) return false;
            bool hit_left = left->hit(ray, ray_t, rec);
            bool hit_right = right->hit(ray, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        aabb bounding_box() const override {return bbox;}
};

#endif