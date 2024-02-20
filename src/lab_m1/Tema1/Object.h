#pragma once

#include <string>
#include <iostream>

#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "Transform.h"

struct Mouse
{
    std::pair<float, float> pos;
    bool isLeftButtonActive = false;
    int nrLeftButtonClick = 0;
    int nrRightButtonClick = 0;
};

class Object
{
public:
    Object() : mesh(nullptr), color(glm::vec3(1.0f)), translate(std::make_pair(0.0f, 0.0f)), scale(std::make_pair(1.0f, 1.0f)), rotate(0.0f), modelMatrix(glm::mat3(1.0f)) {}
    Object(Mesh* newMesh, glm::vec3 newColor, std::pair<float, float> newPos)
        : mesh(newMesh), color(newColor), pos(newPos), translate(newPos), scale(std::make_pair(1, 1)), rotate(0), modelMatrix(glm::mat3(1)), isClicked(false) {}
    ~Object() {}
    virtual void PutObject()
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(pos.first, pos.second);
    }

    Mesh* mesh;
    glm::vec3 color;
    std::pair<float, float> pos;
    std::pair<float, float> translate;
    std::pair<float, float> scale;
    float rotate;
    glm::mat3 modelMatrix;
    bool isClicked;
};

class Circle : public Object
{
public:
    Circle() : Object() {}
    Circle(std::pair<float, float> newPos) :
        Object(nullptr, glm::vec3(1.0f), newPos) {}
    ~Circle() {}
};

class Square : public Object
{
public:
    Square() : Object() {}
    Square(Mesh* newMesh, glm::vec3 newColor, std::pair<float, float> newPos) :
        Object(newMesh, newColor, newPos) {}
    ~Square() {}
};

class Rectangle : public Object
{
public:
    Rectangle() : Object() {}
    Rectangle(Mesh* newMesh, glm::vec3 newColor, std::pair<float, float> newPos) :
        Object(newMesh, newColor, newPos) {}
    ~Rectangle() {}
};

class Star : public Object
{
public:
    Star() : Object() {}
    Star(Mesh* newMesh, glm::vec3 newColor, std::pair<float, float> newPos) :
        Object(newMesh, newColor, newPos) {}
    ~Star() {}

    void MoveStar(float deltaTimeSeconds);
};

class Points
{
public:
    Points() {}
    Points(Mesh* mesh, glm::vec3 color, std::pair<float, float> start, int nr);
    ~Points() {}

    void AddPoints(int nr);
    void DeletePoints(int nr);

    std::list<Star> points;
};

class Diamond : public Object
{
public:
    Diamond() : Object() {}
    Diamond(Mesh* diamondMesh, glm::vec3 newColor, std::pair<float, float> newPos) :
        Object(diamondMesh, newColor, newPos) {}
    ~Diamond() {}
};

class Canon
{
public:
    Canon() {}
    Canon(Mesh* newMesh, Canon oldCanon) :
        canon(Diamond::Diamond(newMesh, oldCanon.canon.color, oldCanon.canon.translate)),
        missle(oldCanon.missle), nrPoints(oldCanon.nrPoints) {}
    Canon(Mesh* diamondMesh1, Mesh* starMesh, glm::vec3 newColor, std::pair<float, float> newPos, int nr) :
        canon(Diamond::Diamond(diamondMesh1, newColor, newPos)),
        missle(Star::Star(starMesh, newColor, newPos)), nrPoints(nr) {}
    ~Canon() {}

    Diamond canon;
    Star missle;
    int nrPoints;
};

class Enemy : public Object
{
public:
    Enemy() : Object() {}
    Enemy(Mesh* newMesh, glm::vec3 newColor, std::pair<float, float> newPos) :
        Object(newMesh, newColor, newPos), nrHits(0) {}
    ~Enemy() {}

    void MoveEnemy(float deltaTimeSeconds);
    void KillEnemy(float deltaTimeSeconds);
    int nrHits;
};

class Enemies
{
public:
    Enemies() {}
    ~Enemies() {}

    void ChooseEnemy();
    std::vector<float> posY;
    std::vector<Mesh*> meshes;
    std::vector<glm::vec3> colors;
    std::vector<Enemy> enemies;
};
