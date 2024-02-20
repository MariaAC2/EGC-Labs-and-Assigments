#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace drawing
{
    // patrat
    Mesh* CreateSquare(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color, bool fill = false);
    // dreptunghi
    Mesh* CreateRectangle(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color);
    // inamic
    Mesh* CreateHexagon(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color);
    // stea
    Mesh* CreateStar(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color);
    // romb
    Mesh* CreateDiamond(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color);
    // inima
    Mesh* CreateHeart(const std::string& name, glm::vec3 centerPoint, float length, glm::vec3 color);
}