#include "Drawing.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* drawing::CreateSquare(
    const std::string& name,
    glm::vec3 centerPoint,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 center = centerPoint;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-(length / 2), -(length / 2), 0), color),
        VertexFormat(center + glm::vec3(length / 2, -(length / 2), 0), color),
        VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color),
        VertexFormat(center + glm::vec3(-(length / 2), length / 2, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* drawing::CreateRectangle(
    const std::string& name,
    glm::vec3 centerPoint,
    float length,
    glm::vec3 color)
{
    glm::vec3 center = centerPoint;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-length / 4, -(length * 3 + 100) / 2, 40), color),
        VertexFormat(center + glm::vec3(length / 4, -(length * 3 + 100) / 2, 40), color),
        VertexFormat(center + glm::vec3(length / 4, (length * 3 + 100) / 2, 40), color),
        VertexFormat(center + glm::vec3(-length / 4, (length * 3 + 100) / 2, 40), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = 
    { 
        0, 1, 2, 
        3, 0, 2 
    };

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* drawing::CreateDiamond(
    const std::string& name,
    glm::vec3 centerPoint,
    float length,
    glm::vec3 color)
{
    glm::vec3 center = centerPoint;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-length / 5, 0, 10), color),
        VertexFormat(center + glm::vec3(0, -length * 2 / 5, 10), color),
        VertexFormat(center + glm::vec3(length / 5, 0, 10), color),
        VertexFormat(center + glm::vec3(0, length * 2 / 5, 10), color),
        VertexFormat(center + glm::vec3(0, -length / 10, 10), color),
        VertexFormat(center + glm::vec3(length / 2, -length / 10, 10), color),
        VertexFormat(center + glm::vec3(length / 2, length / 10, 10), color),
        VertexFormat(center + glm::vec3(0, length / 10, 10), color)
    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        3, 0, 2,
        4, 5, 6,
        7, 4, 6
    };

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* drawing::CreateHexagon(
    const std::string& name,
    glm::vec3 centerPoint,
    float length,
    glm::vec3 color)
{
    glm::vec3 center = centerPoint;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-length * 2 / 5, 0, 10), color), //0
        VertexFormat(center + glm::vec3(-length / 5, -length * 2 / 5, 10), color),//1
        VertexFormat(center + glm::vec3(length / 5, -length * 2 / 5, 10), color),///2
        VertexFormat(center + glm::vec3(length * 2 / 5, 0, 10), color),//3
        VertexFormat(center + glm::vec3(length / 5, length * 2 / 5, 10), color),//4
        VertexFormat(center + glm::vec3(-length / 5, length * 2 / 5, 10), color),//5
        VertexFormat(center, color),//6

        VertexFormat(center + glm::vec3(-length / 5, 0, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//7
        VertexFormat(center + glm::vec3(-length / 10, -length / 5, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//8
        VertexFormat(center + glm::vec3(length / 10, -length / 5, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//9
        VertexFormat(center + glm::vec3(length / 5, 0, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//10
        VertexFormat(center + glm::vec3(length / 10, length / 5, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//11
        VertexFormat(center + glm::vec3(-length / 10, length / 5, 20), glm::vec3(0.231f, 0.705f, 0.635f)),//12
        VertexFormat(center + glm::vec3(0, 0, 20), glm::vec3(0.231f, 0.705f, 0.635f))//13
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        6, 0, 1,
        6, 1, 2,
        6, 2, 3,
        6, 3, 4,
        6, 4, 5,
        6, 5, 0,
        13, 7, 8,
        13, 8, 9,
        13, 9, 10,
        13, 10, 11,
        13, 11, 12,
        13, 12, 7
    };

    hexagon->InitFromData(vertices, indices);

    return hexagon;
}

Mesh* drawing::CreateStar(
    const std::string& name,
    glm::vec3 centerPoint,
    float length,
    glm::vec3 color)
{
    glm::vec3 center = centerPoint;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(0, length / 2, 30), color), //0
        VertexFormat(center + glm::vec3(-length / 10, length / 5, 30), color), //1
        VertexFormat(center + glm::vec3(-length * 2 / 5, length / 5, 30), color), //2
        VertexFormat(center + glm::vec3(-length / 5, 0, 30), color),//3
        VertexFormat(center + glm::vec3(-length * 3 / 10, -length * 3 / 10, 30), color),//4
        VertexFormat(center + glm::vec3(0, -length / 10, 30), color),//5
        VertexFormat(center + glm::vec3(length * 3 / 10, -length * 3 / 10, 30), color),//6
        VertexFormat(center + glm::vec3(length / 5, 0, 30), color),//7
        VertexFormat(center + glm::vec3(length * 2 / 5, length / 5, 30), color),//8
        VertexFormat(center + glm::vec3(length / 10, length / 5, 30), color),//9
        VertexFormat(center, color),//10
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 9,
        1, 9, 10,
        1, 2, 3,
        1, 3, 10,
        3, 4, 5,
        3, 5, 10,
        5, 6, 7,
        5, 7, 10,
        7, 8, 9,
        7, 9, 10
    };

    star->InitFromData(vertices, indices);

    return star;
}

Mesh* drawing::CreateHeart(
    const std::string& name,
    glm::vec3 centerPoint,
    float size,
    glm::vec3 color)
{
    glm::vec3 center = centerPoint;

    // Define the number of points to approximate the circle
    const int numPoints = 50;

    std::vector<VertexFormat> vertices;

    // Create the left semi-circle
    for (int i = 0; i <= numPoints; ++i) {
        float angle = static_cast<float>(i) / numPoints * glm::pi<float>();
        float x = -size * glm::sin(angle) + center.x;
        float y = size * glm::cos(angle) + center.y;
        vertices.emplace_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

    // Create the right semi-circle
    for (int i = 0; i <= numPoints; ++i) {
        float angle = static_cast<float>(i) / numPoints * glm::pi<float>();
        float x = size * glm::sin(angle) + center.x;
        float y = size * glm::cos(angle) + center.y;
        vertices.emplace_back(VertexFormat(glm::vec3(x, y, 0), color));
    }

    Mesh* heartMesh = new Mesh(name);

    // Combine the vertices of both semi-circles to form the heart
    heartMesh->InitFromData(vertices, {}); // No need for indices; vertices form continuous strips

    return heartMesh;
}