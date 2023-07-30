#ifndef OBJECTS_LIST_H
#define OBJECTS_LIST_H

#include "object.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class ObjectsList : public Object
{
    public:
        std::vector<shared_ptr<Object>> objects;
        ObjectsList() {}
        ObjectsList(shared_ptr<Object> obj) {add(obj);}
        void clear() {objects.clear();}
        void add(shared_ptr<Object> obj) {objects.push_back(obj);}
        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool ObjectsList::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_flag = false;
    double closest_t = t_max;

    for (const auto& obj : objects)
    {
        if (obj->hit(ray, t_min, closest_t, temp_rec))
        {
            hit_flag = true;
            closest_t = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_flag;
}

#endif