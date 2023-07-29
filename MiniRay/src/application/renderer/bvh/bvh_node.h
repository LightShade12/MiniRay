#pragma once

#include "application/renderer/scene.h"
#include "UtilsCommon/RayTraceUtils.h"

#include <vector>
#include <memory>

using point3 = glm::vec3;

class aabb {
public:
	aabb() {}
	aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

	bool hit(const Ray& r, float t_min, float t_max) const;

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	point3 minimum;
	point3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1);

struct trianglecluster
{
public:
	std::vector<Triangle>triangles;
	int ModelIdx = 0;
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
