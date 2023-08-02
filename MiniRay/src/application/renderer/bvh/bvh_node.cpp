#include "bvh_node.h"


aabb surrounding_box(aabb box0, aabb box1)
{
    glm::vec3 small(fmin(box0.min().x, box1.min().x),
        fmin(box0.min().y, box1.min().y),
        fmin(box0.min().z, box1.min().z));

    glm::vec3 big(fmax(box0.max().x, box1.max().x),
        fmax(box0.max().y, box1.max().y),
        fmax(box0.max().z, box1.max().z));

    return aabb(small, big);
}

bvh_node::bvh_node()
{
}
//
//inline bool aabb::hit(const Ray& r, float t_min, float t_max) const
//{
//
//    for (int a = 0; a < 3;) {
//        auto invD = 1.0f / r.dir[a];
//        auto t0 = (min()[a] - r.orig[a]) * invD;
//        auto t1 = (max()[a] - r.orig[a]) * invD;
//        if (invD < 0.0f)
//            std::swap(t0, t1);
//        t_min = t0 > t_min ? t0 : t_min;
//        t_max = t1 < t_max ? t1 : t_max;
//        if (t_max <= t_min)
//            return false;
//    }
//    return true;
//};
//
