#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <utility>
#include <iostream>

#include "Transform.h"
#include "Drawing.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    InitTerrain();
    InitInterface();
    InitEnemies();
    InitStars();
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    RenderTerrains();
    RenderInterface();
    RenderStars();
    int deleteEnemyPos = -1;
    int deleteCanonPos = -1;

    render_stars_time += deltaTimeSeconds;

    if (render_stars_time >= 8)
    {
        InitStars();
        render_stars_time = 0;
    }

    float min_time = 2.0f;
    float max_time = 6.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min_time, max_time);

    if (render_enemy_time + dis(gen) <= glfwGetTime())
    {
        enemies.ChooseEnemy();
        render_enemy_time += dis(gen);
    }

    for (int i = 0; i < enemies.enemies.size(); i++) {
        enemies.enemies[i].modelMatrix = glm::mat3(1);
        enemies.enemies[i].modelMatrix *= transform2D::Translate(enemies.enemies[i].translate.first, enemies.enemies[i].translate.second);

        if (enemies.enemies[i].nrHits < 3) {
            enemies.enemies[i].translate.first -= deltaTimeSeconds * 100;

            if (enemies.enemies[i].translate.first <= border.pos.first)
            {
                numberOfLives--;
                deleteEnemyPos = i;

                if (numberOfLives > 0)
                {
                    Mesh* death = drawing::CreateSquare("death", center, squareSide, black, true);
                    lives[numberOfLives].mesh = death;
                    lives[numberOfLives].color = black;
                }
                else
                {
                    window->Close();
                }
            }
        }
        else if (enemies.enemies[i].nrHits == 3) {
            enemies.enemies[i].scale.first += deltaTimeSeconds * 20;
            enemies.enemies[i].scale.second += deltaTimeSeconds * 20;

            enemies.enemies[i].modelMatrix *= transform2D::Scale(1 / enemies.enemies[i].scale.first, 1 / enemies.enemies[i].scale.second);

            if (1 / enemies.enemies[i].scale.first <= 0.5f && 1 / enemies.enemies[i].scale.second <= 0.5f)
            {
                deleteEnemyPos = i;
            }
        }

        RenderMesh2D(enemies.enemies[i].mesh, shaders["VertexColor"], enemies.enemies[i].modelMatrix);
    }

    if (mouse.isLeftButtonActive)
    {
        canon.canon.modelMatrix = glm::mat3(1);
        canon.canon.modelMatrix *= transform2D::Translate(canon.canon.translate.first, canon.canon.translate.second);
        RenderMesh2D(canon.canon.mesh, shaders["VertexColor"], canon.canon.modelMatrix);
    }

    for (auto& canonEntry : canons_in_use)
    {
        canonEntry.second.canon.modelMatrix = glm::mat3(1);
        canonEntry.second.canon.modelMatrix *= transform2D::Translate(canonEntry.second.canon.translate.first, canonEntry.second.canon.translate.second);

        if (canonEntry.second.canon.isClicked)
        {
            canonEntry.second.canon.scale.first += deltaTimeSeconds * 20;
            canonEntry.second.canon.scale.second += deltaTimeSeconds * 20;

            canonEntry.second.canon.modelMatrix *= transform2D::Scale(1 / canonEntry.second.canon.scale.first, 1 / canonEntry.second.canon.scale.second);
            RenderMesh2D(canonEntry.second.canon.mesh, shaders["VertexColor"], canonEntry.second.canon.modelMatrix);

            if (1 / canonEntry.second.canon.scale.first <= 0.5f && 1 / canonEntry.second.canon.scale.second <= 0.5f)
            {
                deleteCanonPos = canonEntry.first;
                canonEntry.second.canon.isClicked = false;
            }
        }
        else
        {
            canonEntry.second.missle.modelMatrix = glm::mat3(1);
            canonEntry.second.missle.rotate -= deltaTimeSeconds * 5;
            canonEntry.second.missle.modelMatrix *= transform2D::Translate(canonEntry.second.missle.translate.first, canonEntry.second.missle.translate.second);
            canonEntry.second.missle.modelMatrix *= transform2D::Rotate(canonEntry.second.missle.rotate);

            float posY = canonEntry.second.canon.pos.second;
            glm::vec3 color = canonEntry.second.missle.color;

            for (int i = 0; i < enemies.enemies.size(); i++)
            {
                if (enemies.enemies[i].pos.second == posY)
                {
                    if (enemies.enemies[i].color == color)
                    {
                        RenderMesh2D(canonEntry.second.missle.mesh, shaders["VertexColor"], canonEntry.second.missle.modelMatrix);

                        if (glm::distance(enemies.enemies[i].translate.first, canonEntry.second.missle.translate.first) <= squareSide)
                        {
                            canonEntry.second.missle.translate.first = canonEntry.second.missle.pos.first;
                            enemies.enemies[i].nrHits++;
                        }
                    }

                    if (glm::distance(enemies.enemies[i].translate.first, canonEntry.second.canon.pos.first) <= squareSide)
                    {
                        canonEntry.second.canon.scale.first += deltaTimeSeconds * 20;
                        canonEntry.second.canon.scale.second += deltaTimeSeconds * 20;

                        canonEntry.second.canon.modelMatrix *= transform2D::Scale(1 / canonEntry.second.canon.scale.first, 1 / canonEntry.second.canon.scale.second);
                        RenderMesh2D(canonEntry.second.canon.mesh, shaders["VertexColor"], canonEntry.second.canon.modelMatrix);

                        if (1 / canonEntry.second.canon.scale.first <= 0.5f && 1 / canonEntry.second.canon.scale.second <= 0.5f)
                        {
                            deleteCanonPos = canonEntry.first;
                        }

                        enemies.enemies[i].scale.first += deltaTimeSeconds * 20;
                        enemies.enemies[i].scale.second += deltaTimeSeconds * 20;

                        enemies.enemies[i].modelMatrix *= transform2D::Scale(1 / enemies.enemies[i].scale.first, 1 / enemies.enemies[i].scale.second);

                        if (1 / enemies.enemies[i].scale.first <= 0.5f && 1 / enemies.enemies[i].scale.second <= 0.5f)
                        {
                            deleteEnemyPos = i;
                        }
                    }
                }
            }

            RenderMesh2D(canonEntry.second.canon.mesh, shaders["VertexColor"], canonEntry.second.canon.modelMatrix);
            if (canonEntry.second.missle.translate.first < 1280)
            {
                canonEntry.second.missle.translate.first += deltaTimeSeconds * 150;
                RenderMesh2D(canonEntry.second.missle.mesh, shaders["VertexColor"], canonEntry.second.missle.modelMatrix);
            }
        }
    }

    if (deleteEnemyPos != -1)
    {
        enemies.enemies.erase(enemies.enemies.begin() + deleteEnemyPos);
    }

    if (deleteCanonPos != -1)
    {
        canons_in_use.erase(deleteCanonPos);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mouse.pos.first = mouseX;
    mouse.pos.second = 720 - mouseY;

    if (mouse.isLeftButtonActive)
    {
        canon.canon.translate = mouse.pos;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    if (button == GLFW_MOUSE_BUTTON_2)
    {
        for (int i = 0; i < 4; i++)
        {
            int diff = score.points.size() - canons[i].nrPoints;
            if (glm::distance(mouse.pos.first, outlines[i].pos.first) <= squareSide / 2
                && glm::distance(mouse.pos.second, outlines[i].pos.second) <= squareSide / 2
                && diff >= 0)
            {
                mouse.isLeftButtonActive = true;
                Mesh* new_mesh = canons[i].canon.mesh;
                AddMeshToList(new_mesh);

                canon = Canon::Canon(new_mesh, canons[i]);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            if (glm::distance(mouse.pos.first, stars[i].pos.first) <= squareSide / 4
                && glm::distance(mouse.pos.second, stars[i].pos.second) <= squareSide / 4)
            {
                stars[i].isClicked = true;
                score.AddPoints(1);
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_3)
    {
        for (int i = 0; i < 9; i++)
        {
            if (glm::distance(mouse.pos.first, terrains[i].pos.first) <= squareSide / 2
                && glm::distance(mouse.pos.second, terrains[i].pos.second) <= squareSide / 2)
            {
                canons_in_use.find(i)->second.canon.isClicked = true;
            }
        }
    }
    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    int key = 0;
    std::pair<float, float> posSquare;
    if (mouse.isLeftButtonActive == true)
    {
        for (int i = 0; i < terrains.size(); i++)
        {
            if (glm::distance(mouse.pos.first, terrains[i].pos.first) <= squareSide / 2
                && glm::distance(mouse.pos.second, terrains[i].pos.second) <= squareSide / 2)
            {
                mouse.isLeftButtonActive = false;
                key = i;
                posSquare = terrains[i].pos;

                if (key != -1 && canons_in_use.find(key) == canons_in_use.end())
                {
                    canon.canon.pos = canon.canon.translate = posSquare;
                    canon.missle.pos.first = canon.missle.translate.first = posSquare.first + 50;
                    canon.missle.pos.second = canon.missle.translate.second = posSquare.second;

                    canons_in_use[key] = canon;
                    score.DeletePoints(canon.nrPoints);
                }
            }
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
