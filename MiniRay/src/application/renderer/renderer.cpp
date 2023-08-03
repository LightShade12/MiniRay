#include "renderer.h"
#include <iostream>
#include "UtilsCommon/random.h"
#include "UtilsCommon/RayTraceUtils.h"

//will only handle one mesh containing models
aabb mesh_bounding_box(const MeshModel& model)
{
	auto mesh = model.m_Meshes[0];///iykyk
	auto verts = mesh.m_vertices;

	std::vector<glm::vec3>tempvertices;//vector of coordinates of vertices

	//vertex creation
	for (size_t i = 0; i < verts.size(); i += 3)
	{
		glm::vec3 vertex(verts[i], verts[i + 1], verts[i + 2]);
		tempvertices.push_back(vertex);
	}
	glm::vec3 min = tempvertices[0], max = tempvertices[1];

	for (auto vertex : tempvertices)
	{
		if (vertex.x < min.x)min.x = vertex.x;
		if (vertex.y < min.y)min.y = vertex.y;
		if (vertex.z < min.z)min.z = vertex.z;

		if (vertex.x > max.x)max.x = vertex.x;
		if (vertex.y > max.y)max.y = vertex.y;
		if (vertex.z > max.z)max.z = vertex.z;
	}

	return aabb(min, max);
}

static bool shouldbuildbvh = true;

int threshold_triangles_count = 3;

inline bool box_compare(const MeshModel& a, const MeshModel& b, int axis) {
	aabb box_a;
	aabb box_b;

	box_a = mesh_bounding_box(a);
	box_b = mesh_bounding_box(b);

	return box_a.min()[axis] < box_b.min()[axis];
}

bool box_x_compare(const MeshModel& a, const MeshModel& b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const MeshModel& a, const MeshModel& b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const MeshModel& a, const MeshModel& b) {
	return box_compare(a, b, 2);
}

//for child call
void buildbvh(const Scene& scene, const std::shared_ptr<bvh_node>& rootnode, int start, int end);

//for parent call
void buildbvh(const Scene& scene, const std::shared_ptr<bvh_node>& rootnode)
{
	buildbvh(scene, rootnode, 0, scene.Models.size());
}

void buildbvh(const Scene& scene, const std::shared_ptr<bvh_node>& rootnode, int start, int end)
{
	auto src_objects = scene.Models;

	int axis = Random::UInt(0, 2);//make this constant for unidirectional bvh splitting
	auto comparator = (axis == 0) ? box_x_compare
		: (axis == 1) ? box_y_compare
		: box_z_compare;

	int span = end - start;//responsible for depth; relative
	if (span <= 0)std::cerr << "span invalid\n";

	trianglecluster temptriclus;
	//multi models/mesh input
	if (span > 1) {
		//sorting? implement?
		std::sort(src_objects.begin() + start, src_objects.begin() + end, comparator);

		int mid = start + span / 2;//new end idx

		if (rootnode == nullptr)std::cerr << "rootnode is null\nrootnode location: " << rootnode.get() << "\n";

		auto leftnode = std::make_shared<bvh_node>();
		buildbvh(scene, leftnode, start, mid);
		rootnode->m_leftchildnode = leftnode;

		auto rightnode = std::make_shared<bvh_node>();
		buildbvh(scene, rightnode, mid, end);//4,8	6,8	   7,8
		rootnode->m_rightchildnode = rightnode;
	}
	//single mesh input
	else
	{
		temptriclus.ModelIdx = start;
		temptriclus.MeshIdx = 0;
		temptriclus.triangles = scene.Models[temptriclus.ModelIdx].m_Meshes[temptriclus.MeshIdx].m_triangles;

		if (rootnode == nullptr)std::cerr << "rootnode is null\nrootnode location: " << rootnode.get() << "\n";

		rootnode->m_trianglecluster = temptriclus;
	}

	//bvhnode
	if (rootnode->m_trianglecluster.triangles.empty())
	{
		rootnode->node_bounding_volume = surrounding_box(rootnode->m_leftchildnode->node_bounding_volume, rootnode->m_rightchildnode->node_bounding_volume);
	}
	else//leafnode
	{
		rootnode->node_bounding_volume = mesh_bounding_box(scene.Models[temptriclus.ModelIdx]);
	}
}

void renderer::render(const Scene& scene, const Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	if (shouldbuildbvh)
	{
		//build bvh
		m_Scenebvh = std::make_shared<bvh_node>();//move to constructor
		//printf("scenebvh location:%d\n", m_Scenebvh.get());
		buildbvh(*m_ActiveScene, m_Scenebvh);

		shouldbuildbvh = false;
	}

	if (m_FrameIndex == 1)
		memset(m_accumulationbuffer.data(), 0, m_FinalImage->GetHeight() * m_FinalImage->GetWidth() * sizeof(glm::vec3));

	if (m_FrameIndex == m_Settings.MaxSamplesLimit)
		return;

	for (int y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (int x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec3 color = RayGen(x, y);
			//glm::vec2 coord = { (float)x / m_FinalImage->GetWidth(),(float)y / m_FinalImage->GetHeight() };
			//coord.x *= ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//aspect ratio
			//coord.x = coord.x * 2.0f - ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//remap with aspect
			//coord.y = coord.y * 2.0f - 1.0f;//remap
			m_accumulationbuffer[x + y * m_FinalImage->GetWidth()] += color;
			glm::vec3 AccumulatedColor = m_accumulationbuffer[x + y * m_FinalImage->GetWidth()];
			AccumulatedColor /= (float)m_FrameIndex;
			m_rawbuffer[x + y * m_FinalImage->GetWidth()] = AccumulatedColor;//for some reason it works like its normalised; maybe tonemapper will fix?
		}
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());

	if (m_Settings.Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;//if at fID 10 then go back to 1 edit:wtf does this mean
}

float rayEpsilon = 0.001f;

//Executes Perpixel; currently also handles shading
glm::vec3 renderer::RayGen(uint32_t x, uint32_t y)
{
	//ray
	Ray ray;
	//ray.orig = m_ActiveCamera->GetPosition() - glm::vec3(0,-1,0);
	ray.orig = m_ActiveCamera->GetPosition();
	ray.dir = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	//shading variables
	glm::vec3 light(0);
	glm::vec3 contribution(1.0f);//models semi spectral absorption;should be 1
	glm::vec3 sun_position(1, 2, 1);
	glm::vec3 sun_color(1, 0.7, 0.3);

	//seed generation
	uint32_t seed = x + y * m_FinalImage->GetWidth();//local pixel seed
	seed *= m_FrameIndex;

	//recursive ray generation & shading
	//for (int i = 0; i < m_Settings.Bounces; i++)
	for (int i = 0; i < 1; i++)
	{
		seed += i;

		HitPayload payload = TraceRay(ray);//1

		//Miss shader color------------------------------
		if (payload.HitDistance < 0)
		{
			glm::vec3 skycolor(0.6, 0.7, 0.9);
			//light += skycolor * contribution;
			light = skycolor;
			break;
		}
		//closestHit shader color------------------------------
		const Mesh& mesh = m_ActiveScene->Models[payload.ModelIndex].m_Meshes[0];
		const Material& material = m_ActiveScene->Materials[mesh.MaterialIndex];//material selection
		//light += material.GetEmmision();
		//light = { 1, 0, 0 };
		light = material.Albedo;
		//contribution *= material.Albedo;

		//new ray generation
		ray.orig = payload.WorldPosition + (payload.WorldNormal * rayEpsilon);

		//shadowray
		HitPayload shadowpayload = TraceRay(Ray(ray.orig, sun_position + (RayTraceIntern::Vec3(seed, -0.5, 0.5) * 0.15f)));
		if (shadowpayload.HitDistance < 0)
			//light += sun_color * 0.5f;

			if (m_Settings.mt1997_Random)
				ray.dir = glm::normalize(payload.WorldNormal + Random::InUnitSphere());
			else
				ray.dir = glm::normalize(payload.WorldNormal + RayTraceIntern::InUnitSphere(seed));
	}

	return light;
};

void renderer::process(const Ray& ray, HitPayload& workingpayload, const std::shared_ptr<bvh_node>& node, trianglecluster& triclus, bool& leafcheck, bool& geomhit)
{
	//leaf node reached
	if (!(node->m_trianglecluster.triangles.empty()))
	{
		triclus = node->m_trianglecluster;
		leafcheck = true;

		for (const auto& tri : triclus.triangles)
		{
			workingpayload = Intersection(ray, triclus.ModelIdx, workingpayload, tri, triclus.MeshIdx);//in future it will take bottom most "node", payload and ray
		}
		//did intersect
		if (workingpayload.ModelIndex > -1)
		{
			geomhit = true;
		}
		else //didnt intersect
		{
			geomhit = false;
		}
	}
}

//Todo: add closest bvh hit check and handle colinear initial miss
void renderer::preorder(const Ray& ray, HitPayload& workingpayload, const std::shared_ptr<bvh_node>& root, trianglecluster& triclus, bool& leafcheck, bool& geomhit)
{
	//unlikely to be executed
	//if (root.get() == nullptr)return;

	process(ray, workingpayload, root, triclus, leafcheck, geomhit);//check if leaf; get tricluster

	if (leafcheck)
	{
		leafcheck = false;
		return;//to not test for child if on leaf node
	}

	if (root->m_leftchildnode->node_bounding_volume.hit(ray))
		preorder(ray, workingpayload, root->m_leftchildnode, triclus, leafcheck, geomhit);
	if (geomhit)return;//to not continue search if confirmed geometry hit

	if (root->m_rightchildnode->node_bounding_volume.hit(ray))
		preorder(ray, workingpayload, root->m_rightchildnode, triclus, leafcheck, geomhit);
	//if (leafcheck)return;//to not continue search if leaf found
}

//shoots ray into the scene; engine responsible for traversing the ray throughout the scene
HitPayload renderer::TraceRay(const Ray& ray)
{
	//initialise working payload
	HitPayload WorkingPayload;
	WorkingPayload.ModelIndex = -1;//object index of closest sphere
	WorkingPayload.HitDistance = FLT_MAX;

	//TODO:tranverse accel structures here
	trianglecluster selected_triangles;
	bool leaf_node_reached = false;
	bool geometry_hit = false;
	std::vector<std::shared_ptr<bvh_node>>childnodes;
	std::vector<std::shared_ptr<bvh_node>>hitnodes;//colinear hits

	//printf("min:%.2f x %.2f x %.2f \n", m_Scenebvh->node_bounding_volume.min().x, m_Scenebvh->node_bounding_volume.min().y, m_Scenebvh->node_bounding_volume.min().z);
	//printf("max:%.2f x %.2f x %.2f \n", m_Scenebvh->node_bounding_volume.max().x, m_Scenebvh->node_bounding_volume.max().y, m_Scenebvh->node_bounding_volume.max().z);

	if (m_Scenebvh->node_bounding_volume.hit(ray))
	{
		//printf("hit scenebvh\n");
		preorder(ray, WorkingPayload, m_Scenebvh, selected_triangles, leaf_node_reached, geometry_hit);
	}

	//looping over scene objects
	//im future, with tlas and blas or simple bvh, this loop might iterate over triangles of the bottom most node in accel tree inside the intersection shader
	//if (false)
	//if (selected_triangles.ModelIdx > -1)
	//	for (const auto& tri : selected_triangles.triangles)
	//	{
	//		WorkingPayload = Intersection(ray, selected_triangles.ModelIdx, WorkingPayload, tri, selected_triangles.MeshIdx);//in future it will take bottom most "node", payload and ray
	//	}

	//branched shaders; WHO INVOKES THESE SHADERS? INTERSECTION?
	if (WorkingPayload.ModelIndex < 0) return Miss(ray);//MissShader

	return ClosestHit(ray, WorkingPayload.HitDistance, WorkingPayload.ModelIndex, WorkingPayload.triangle, WorkingPayload.MeshIndex);//ClosestHitShader
}

//closesthitshader; configures variables required for shading
//NOTE: its not generalized; integrated with sphere
HitPayload renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex, const Triangle& closestTriangle, int meshindex)
{
	//setup
	HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ModelIndex = objectIndex;

	//calculation
	//glm::vec3 origin = ray.orig - closestTriangle.Position;//add matrix translation code here

	//payload.WorldPosition = ray.orig + ray.dir * hitDistance;//originally used origin for translation
	payload.WorldPosition = ray.orig + ray.dir * payload.HitDistance;

	//generate normals at runtime or use precomputed
	if (true)
	{
		glm::vec3 edge1 = closestTriangle.vert1 - closestTriangle.vert0;
		glm::vec3 edge2 = closestTriangle.vert2 - closestTriangle.vert0;
		payload.WorldNormal = (glm::cross(edge1, edge2));//swap edges for ccw or cw
		payload.WorldNormal = glm::normalize(payload.WorldNormal);
	}
	else
		payload.WorldNormal = glm::normalize(closestTriangle.Normal);

	return payload;
}

HitPayload renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.HitDistance = -1;
	return payload;
};

// Function to check if two floating-point numbers are almost equal within a small epsilon
bool nearlyEqual(float a, float b, float epsilon = 1e-8) {
	return fabs(a - b) < epsilon;
}

//Ray Triangle Intersector
HitPayload renderer::Intersection(const Ray& ray, int modelidx, const HitPayload& incomingpayload, const Triangle& triangle, int meshindex)
{
	//setup
	HitPayload payload = incomingpayload;

	//calculation

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = triangle.vert1 - triangle.vert0;

	edge2 = triangle.vert2 - triangle.vert0;

	h = glm::cross(ray.dir, edge2);
	a = glm::dot(edge1, h);

	// Check if the ray is parallel to the triangle plane
	if (nearlyEqual(a, 0.0f, rayEpsilon))
		return payload;

	f = 1.0f / a;
	s = ray.orig - triangle.vert0;
	u = f * glm::dot(s, h);

	// Check if the intersection point is outside the triangle
	if (u < 0.0f || u > 1.0f)
		return payload;

	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.dir, q);

	// Check if the intersection point is outside the triangle
	if (v < 0.0f || u + v > 1.0f)
		return payload;

	// Compute the t value, which represents the distance from the ray origin to the intersection point
	float closest_t = f * glm::dot(edge2, q);

	// Check if the intersection point is behind the ray origin or too far away
	if (closest_t < rayEpsilon)
		return payload;

	//not miss
	//if closest intersection
	if (closest_t > 0 && closest_t < incomingpayload.HitDistance) {
		payload.HitDistance = closest_t;
		payload.ModelIndex = modelidx;
		payload.MeshIndex = meshindex;
		payload.triangle = triangle;
	}

	return payload;
}

void renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_renderwidth = width;
		m_renderheight = height;
		m_FinalImage->Resize(width, height);
		m_rawbuffer.resize(width * height, glm::vec3(0));
		m_accumulationbuffer.resize(width * height, glm::vec3(0));
		ResetFrameIndex();
	}
	else
	{
		m_rawbuffer.resize(width * height, glm::vec3(0));//keep some data so gpu can be init
		m_accumulationbuffer.resize(width * height, glm::vec3(0));
		m_FinalImage = std::make_shared<Image>(width, height, GL_RGB32F, m_rawbuffer.data());
	}
}