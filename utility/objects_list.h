#ifndef OBJECTS_LIST_H
#define OBJECTS_LIST_H

#include "object.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class ObjectsList : public Object
{
    private:
        aabb bbox;
    public:
        std::vector<shared_ptr<Object>> objects;
        ObjectsList() {}
        ObjectsList(shared_ptr<Object> obj) {add(obj);}
        void clear() {objects.clear();}
        void add(shared_ptr<Object> obj) 
        {
            objects.push_back(obj);
            bbox = aabb(bbox, obj->bounding_box());
        }
        virtual bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override;
        aabb bounding_box() const override {return bbox;}
};

bool ObjectsList::hit(const Ray& ray, Interval ray_t, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_flag = false;
    double closest_t = ray_t.max;

    for (const auto& obj : objects)
    {
        if (obj->hit(ray, Interval(ray_t.min, closest_t), temp_rec))
        {
            hit_flag = true;
            closest_t = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_flag;
}

#endif