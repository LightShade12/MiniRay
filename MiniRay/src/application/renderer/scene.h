#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "MeshModel.h"

struct Material;
class Mesh;
class MeshModel;

struct Scene
{
	std::vector<MeshModel> Models;
	std::vector<Material> Materials;
};