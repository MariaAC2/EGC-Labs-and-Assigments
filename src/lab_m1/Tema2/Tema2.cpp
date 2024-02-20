#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::Init()
{
    Mesh* mesh1 = new Mesh("senile");
    mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tank"), "senile.obj");
    meshes[mesh1->GetMeshID()] = mesh1;

    Mesh* mesh2 = new Mesh("corp");
    mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tank"), "corp.obj");
    meshes[mesh2->GetMeshID()] = mesh2;

    Mesh* mesh3 = new Mesh("tunela");
    mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tank"), "tunela.obj");
    meshes[mesh3->GetMeshID()] = mesh3;

    Mesh* mesh4 = new Mesh("tun");
    mesh4->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tank"), "tun.obj");
    meshes[mesh4->GetMeshID()] = mesh4;

    Mesh* mesh5 = new Mesh("proiectil");
    mesh5->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tank"), "proiectil.obj");
    meshes[mesh5->GetMeshID()] = mesh5;

    Mesh* cladire = new Mesh("building");
    cladire->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "building.obj");
    meshes[cladire->GetMeshID()] = cladire;

    Mesh* teren = new Mesh("terrain");
    teren->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[teren->GetMeshID()] = teren;

    player = custom::Player(mesh1, mesh2, mesh3, mesh4, mesh5);

    buildings = custom::Buildings(cladire);

    terrain = custom::Object(teren, glm::vec3(0.0f));

    enemies.resize(4);

    enemies[0] = custom::Enemy(mesh1, mesh2, mesh3, mesh4, mesh5, glm::vec3(-3, 0, -3));
    enemies[0].firstTime = enemies[0].SelectTime();

    enemies[1] = custom::Enemy(mesh1, mesh2, mesh3, mesh4, mesh5, glm::vec3(-3, 0, 3));
    enemies[1].firstTime = enemies[1].SelectTime();

    enemies[2] = custom::Enemy(mesh1, mesh2, mesh3, mesh4, mesh5, glm::vec3(3, 0, 3));
    enemies[2].firstTime = enemies[2].SelectTime();

    enemies[3] = custom::Enemy(mesh1, mesh2, mesh3, mesh4, mesh5, glm::vec3(3, 0, -3));
    enemies[3].firstTime = enemies[3].SelectTime();

    renderCameraTarget = false;

    camera = new custom::Camera();
    camera->Set(glm::vec3(0, 1, 3), player.turret.translateCoord, glm::vec3(0, 1, 0));

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.388, 0.812, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    terrain.PutObject();

    player.PutTank();
    player.cooldown += deltaTimeSeconds;
    player.ThrowProjectiles(deltaTimeSeconds);

    buildings.PutBuildings();

    for (int i = 0; i < 4; i++)
    {
        enemies[i].PutTank();

        if (enemies[i].hp > 0)
        {
            enemies[i].MoveEnemy(enemies[i].firstTime, deltaTimeSeconds);
        }
    }

    int deformPos = -1;

    for (int i = 0; i < 4; i++)
    {
        if (CollisionTankTank(player, enemies[i]))
        {
            player.TranslateTank(deltaTimeSeconds);
            camera->MoveForward(-deltaTimeSeconds);
        }

        for (int j = 0; j < 20; j++)
        {
            if (CollisionTankBuilding(player, buildings.buildings[j]))
            {
                player.TranslateTank(deltaTimeSeconds);
                camera->MoveForward(-deltaTimeSeconds);
            }

            if (CollisionTankBuilding(enemies[i], buildings.buildings[j]))
            {
                if (enemies[i].firstMove == custom::Move::Backward)
                {
                    enemies[i].MakeMove(custom::Move::Forward, 0.25);
                }
                else if (enemies[i].firstMove == custom::Move::Forward)
                {
                    enemies[i].MakeMove(custom::Move::Backward, 0.25);
                }
            }

            int deletePos = -1;
            for (int k = 0; k < player.projectiles.size(); k++)
            {
                if (player.projectiles[k].timer >= 4
                    || CollisionBuildingProjectile(buildings.buildings[j], player.projectiles[k]))
                {
                    deletePos = k;
                }
                else if (CollisionTankProjectile(enemies[i], player.projectiles[k]))
                {
                    deletePos = k;
                    deformPos = i;
                }
            }

            if (deletePos != -1)
            {
                player.projectiles.erase(player.projectiles.begin() + deletePos);
            }
        }
    }

    if (deformPos != -1)
    {
        enemies[deformPos].hp--;
    }

    RenderSimpleMesh(player.tracks.mesh, shaders["LabShader"], player.tracks.modelMatrix, glm::vec3(1), player.hp);
    RenderSimpleMesh(player.body.mesh, shaders["LabShader"], player.body.modelMatrix, glm::vec3(0.702, 0, 0.404), player.hp);
    RenderSimpleMesh(player.turret.mesh, shaders["LabShader"], player.turret.modelMatrix, glm::vec3(1, 0, 0.58), player.hp);
    RenderSimpleMesh(player.gun.mesh, shaders["LabShader"], player.gun.modelMatrix, glm::vec3(1), player.hp);

    for (int i = 0; i < player.projectiles.size(); i++)
    {
        RenderSimpleMesh(player.projectiles[i].mesh, shaders["LabShader"], player.projectiles[i].modelMatrix, glm::vec3(0), -1);
    }

    for (int i = 0; i < 4; i++)
    {
        custom::Enemy enemy = enemies[i];
        RenderSimpleMesh(enemy.tracks.mesh, shaders["LabShader"], enemy.tracks.modelMatrix, glm::vec3(1), enemy.hp);
        RenderSimpleMesh(enemy.body.mesh, shaders["LabShader"], enemy.body.modelMatrix, glm::vec3(0.376, 0, 0.631), enemy.hp);
        RenderSimpleMesh(enemy.turret.mesh, shaders["LabShader"], enemy.turret.modelMatrix, glm::vec3(0.596, 0, 1), enemy.hp);
        RenderSimpleMesh(enemy.gun.mesh, shaders["LabShader"], enemy.gun.modelMatrix, glm::vec3(1), enemy.hp);
    }

    for (int i = 0; i < 20; i++)
    {
        RenderSimpleMesh(buildings.buildings[i].mesh, shaders["LabShader"], buildings.buildings[i].modelMatrix, buildings.buildings[i].color, -1);
    }

    RenderSimpleMesh(terrain.mesh, shaders["LabShader"], terrain.modelMatrix, glm::vec3(0.431, 0.275, 0.106), -1);
}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    shader->Use();
    GLint loc_object_color = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(loc_object_color, 1, glm::value_ptr(color));

    GLint loc_hp = glGetUniformLocation(shader->program, "hp");
    glUniform1i(loc_hp, hp);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2;
    if (window->KeyHold(GLFW_KEY_W))
    {
        player.TranslateTank(-deltaTime);
        camera->MoveForward(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        player.TranslateTank(deltaTime);
        camera->MoveForward(-deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A))
    {
        player.RotateTank(deltaTime);
        camera->RotateThirdPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D))
    {
        player.RotateTank(-deltaTime);
        camera->RotateThirdPerson_OY(-deltaTime);
    }

    //if (window->KeyHold(GLFW_KEY_Q))
    //{
    //    camera->RotateThirdPerson_OX(deltaTime);
    //}
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //// Add mouse move event
    float sensitivity = 0.01f;
    player.RotateTurret(-deltaX * 0.01f);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    if (button == GLFW_MOUSE_BUTTON_2)
    {
        player.AddProjectile();
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}