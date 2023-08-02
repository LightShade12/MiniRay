#pragma once

#include "application/renderer/scene.h"
#include "UtilsCommon/RayTraceUtils.h"

#include <vector>
#include <memory>


class aabb {
public:
	aabb() {}
	aabb(const glm::vec3& a, const glm::vec3& b) { minimum = a; maximum = b; }

	//proper decl and impl later
	inline bool hit(const Ray& r, float t_min = FLT_EPSILON, float t_max = FLT_MAX) const
	{
		for (int a = 0; a < 3;) {
			auto invD = 1.0f / r.dir[a];
			auto t0 = (min()[a] - r.orig[a]) * invD;
			auto t1 = (max()[a] - r.orig[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)
				return false;
		}
		return true;
	};

	glm::vec3 min() const { return minimum; }
	glm::vec3 max() const { return maximum; }

	glm::vec3 minimum;
	glm::vec3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1);

struct trianglecluster
{
public:
	std::vector<Triangle>triangles;
	int ModelIdx = -1;
	int MeshIdx = 0;
};

class bvh_node
{
public:
	bvh_node();

	//using shared ptrs, can use raw with delete children deconstructor that recusrively deletes children ptrs
	std::shared_ptr<bvh_node>m_leftchildnode;
	std::shared_ptr<bvh_node>m_rightchildnode;
	aabb node_bounding_volume;
	trianglecluster m_trianglecluster;
};
