#include "RayTraceUtils.h"

bool RayTraceIntern::near_zero(glm::vec3 ray)
{
	// Return true if the vector is close to zero in all dimensions.
	const auto s = 1e-8;
	return (fabs(ray.x) < s) && (fabs(ray.y) < s) && (fabs(ray.z) < s);
}

glm::vec3 RayTraceUtils::randomvec3(uint32_t& seed)
{
	return glm::vec3(RayTraceIntern::RandomFloat(seed), RayTraceIntern::RandomFloat(seed), RayTraceIntern::RandomFloat(seed));
}

glm::vec3 RayTraceUtils::random_in_unit_sphere(uint32_t& seed)
{
	while (true) {
		glm::vec3 p = randomvec3(seed);
		if (glm::dot(p, p) >= 1) continue;
		return p;
	}
}

glm::vec3 RayTraceUtils::random_unit_vector(uint32_t& seed)
{
	return glm::normalize(random_in_unit_sphere(seed));
}

glm::vec3 RayTraceUtils::lambertian_scatter(uint32_t& seed, HitPayload rec)
{
	glm::vec3 scatter_direction = rec.WorldNormal + random_unit_vector(seed);

	// Catch degenerate scatter direction
	if (RayTraceIntern::near_zero(scatter_direction))
		scatter_direction = rec.WorldNormal;
	return scatter_direction;
}