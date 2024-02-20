#include "Object.h"
#include "Drawing.h"
#include "Transform.h"

#include <list>

#include "core/engine.h"
#include "utils/gl_utils.h"

Points::Points(Mesh* mesh, glm::vec3 color, std::pair<float, float> start, int nr)
{
    float startPosX = start.first - 35.0f;
    float startPosY = start.second - 75.0f;

    for (int i = 0; i < nr; i++)
    {
        points.push_back(Star::Star(mesh, color, std::make_pair(startPosX, startPosY)));
        startPosX += 35.0f;
    }
}

void Points::AddPoints(int nr)
{
    Star star = points.back();
    std::pair<float, float> lastPos = star.translate;

    float lastPosX = lastPos.first + 35.0f;
    float lastPosY = lastPos.second;

    for (int i = 0; i < nr; i++)
    {
        points.push_back(Star::Star(star.mesh, star.color, std::make_pair(lastPosX, lastPosY)));

        if (i < nr - 1)
        {
            lastPosX += 35.0f;
        }
    }
}


void Points::DeletePoints(int nr)
{
    if (points.size() >= nr)
    {
        auto it = points.end();
        std::advance(it, -nr);

        points.erase(it, points.end());
    }
}

void Star::MoveStar(float deltaTimeSeconds)
{
    translate.first += deltaTimeSeconds * 200;
}

void Enemy::MoveEnemy(float deltaTimeSeconds)
{
    translate.first -= deltaTimeSeconds * 50;
}

void Enemies::ChooseEnemy()
{
    int randomPosY = rand() % 3;
    int randomMesh = rand() % 4;

    Enemy enemy = Enemy::Enemy(meshes[randomMesh], colors[randomMesh], std::make_pair(1280, posY[randomPosY]));
    
    enemies.push_back(enemy);
}



