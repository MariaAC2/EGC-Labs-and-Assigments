#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ground.png").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ocean2.png").c_str(), GL_REPEAT);
        mapTextures["ocean"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood2.png").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "brick2.png").c_str(), GL_REPEAT);
        mapTextures["brick"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "moon.png").c_str(), GL_REPEAT);
        mapTextures["moon"] = texture;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("boat_base");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "boat_base.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("boat_light");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "boat_light.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("boat_tag");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "boat_tag.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    boats = custom::Boats();

    for (int i = 0; i < 4; i++)
    {
        colors.push_back(boats.boats[i].color);
        positions.push_back(glm::vec3(boats.boats[i].radius * sin(boats.boats[i].angle), 1, boats.boats[i].radius * cos(boats.boats[i].angle)) * glm::vec3(0.5));
    }

    directLightPosition = glm::vec3(-2, 3, -5);
    lighthouse_position = glm::vec3(0, 1.35, 0);
    lighthouse_color = glm::vec3(1, 0, 1);

    direction1 = glm::vec3(0, -0.25, 1);
    direction1 = glm::vec3(0, -0.25, -1);

    GetSceneCamera()->SetPosition(glm::vec3(0, 2, 6));
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    angle2 += deltaTimeSeconds;
    direction1 = glm::vec3(sin(angle2), -0.25, cos(angle2));
    direction2 = glm::vec3(sin(angle2 + RADIANS(180.0f)), -0.25, cos(angle2 + RADIANS(180.0f)));
    // barcute

    for (int i = 0; i < 4; i++)
    {

        if (boats.boats[i].orientation == 0)
        {
            boats.boats[i].angle += boats.boats[i].speed * deltaTimeSeconds;

            if (boats.boats[i].angle >= RADIANS(360))
            {
                boats.boats[i].angle -= RADIANS(360);
            }
        }
        else
        {
            boats.boats[i].angle -= boats.boats[i].speed * deltaTimeSeconds;

            if (boats.boats[i].angle <= 0)
            {
                boats.boats[i].angle += RADIANS(360);
            }
        }

        boats.boats[i].translateCoord = glm::vec3(boats.boats[i].radius * sin(boats.boats[i].angle), 0.15, boats.boats[i].radius * cos(boats.boats[i].angle));
        positions[i] = glm::vec3(boats.boats[i].radius * sin(boats.boats[i].angle), 0.6, boats.boats[i].radius * cos(boats.boats[i].angle)) * glm::vec3(0.5);

        boats.boats[i].base.PutObject2(boats.boats[i].translateCoord, glm::vec3(0.5), boats.boats[i].angle);
        boats.boats[i].light.PutObject2(boats.boats[i].translateCoord, glm::vec3(0.5), boats.boats[i].angle);

        if (boats.boats[i].orientation == 0)
        {
            boats.boats[i].tag.PutObject2(boats.boats[i].translateCoord, glm::vec3(0.5), boats.boats[i].angle);
        }
        else
        {
            boats.boats[i].tag.PutObject2(boats.boats[i].translateCoord, glm::vec3(0.5), boats.boats[i].angle + RADIANS(180));
        }
    }

    //boats.PutBoats();

    for (int i = 0; i < 4; i++)
    {
        RenderSimpleMesh(meshes["boat_base"], shaders["LabShader"], boats.boats[i].base.modelMatrix, mapTextures["wood"]);
        RenderSimpleMesh(meshes["boat_light"], shaders["LabShader"], boats.boats[i].light.modelMatrix, NULL, boats.boats[i].color);
        RenderSimpleMesh(meshes["boat_tag"], shaders["LabShader"], boats.boats[i].tag.modelMatrix, mapTextures["ocean"]);
    }

    // ocean
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["ocean"]);
    }

    // pamant
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3, 1.3, 1.3));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["ground"]);
    }

    // baza lighthouse
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.25, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 1, 0.3));
        RenderSimpleMesh(meshes["cylinder"], shaders["LabShader"], modelMatrix, mapTextures["brick"]);
    }

    // lumina
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.25, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.2, 0.3));
        RenderSimpleMesh(meshes["cylinder"], shaders["LabShader"], modelMatrix, NULL, lighthouse_color);
    }

    // varf lighthouse
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.45, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.2, 0.3));
        RenderSimpleMesh(meshes["cylinder"], shaders["LabShader"], modelMatrix, mapTextures["brick"]);
    }

    // luna
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, directLightPosition - glm::vec3(0, 1, 0));
        //modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3, 1.3, 1.3));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["moon"]);
    }
}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, glm::vec3 color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // TODO(student): Set light position uniform

    GLint loc_lighthouse_color = glGetUniformLocation(shader->program, "lighthouse_color");
    glUniform3fv(loc_lighthouse_color, 1, glm::value_ptr(lighthouse_color));

    GLint loc_light_pos = glGetUniformLocation(shader->program, "lighthouse_position");
    glUniform3fv(loc_light_pos, 1, glm::value_ptr(lighthouse_position));

    GLint loc_light_pos1 = glGetUniformLocation(shader->program, "direction1");
    glUniform3fv(loc_light_pos1, 1, glm::value_ptr(direction1));

    GLint loc_light_pos2 = glGetUniformLocation(shader->program, "direction2");
    glUniform3fv(loc_light_pos2, 1, glm::value_ptr(direction2));

    GLint loc_angle1 = glGetUniformLocation(shader->program, "cut_off");
    glUniform1f(loc_angle1, angle1);

    if (mesh == meshes["cylinder"] && !texture1)
    {
        GLint loc_is_light = glGetUniformLocation(shader->program, "isLight");
        glUniform1i(loc_is_light, 1);
    }
    else
    {
        GLint loc_is_light = glGetUniformLocation(shader->program, "isLight");
        glUniform1i(loc_is_light, 0);
    }

    GLint loc_direct_light_pos = glGetUniformLocation(shader->program, "direct_light_position");
    glUniform3fv(loc_direct_light_pos, 1, glm::value_ptr(directLightPosition));

    if (texture1 != mapTextures["moon"])
    {
        GLint loc_is_moon = glGetUniformLocation(shader->program, "isMoon");
        glUniform1i(loc_is_moon, 0);
    }
    else
    {
        GLint loc_is_moon = glGetUniformLocation(shader->program, "isMoon");
        glUniform1i(loc_is_moon, 1);
    }

    GLint loc_light_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc_light_color, 1, glm::value_ptr(color));

    if (mesh == meshes["plane"])
    {
        GLint loc_isOcean = glGetUniformLocation(shader->program, "isOcean");
        glUniform1f(loc_isOcean, 1);

        GLint loc_color = glGetUniformLocation(shader->program, "colors");
        glUniform3fv(loc_color, 4, glm::value_ptr(colors[0]));

        GLint loc_position = glGetUniformLocation(shader->program, "positions");
        glUniform3fv(loc_position, 4, glm::value_ptr(positions[0]));

        GLint loc_time = glGetUniformLocation(shader->program, "time");
        glUniform1f(loc_time, Engine::GetElapsedTime());
    }
    else
    {
        GLint loc_isOcean = glGetUniformLocation(shader->program, "isOcean");
        glUniform1f(loc_isOcean, 0);

        GLint loc_time = glGetUniformLocation(shader->program, "time");
        glUniform1f(loc_time, -1.0f);
    }

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform

    GLint loc_eye_pos = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_pos, 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)

    GLint loc_material_Kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(loc_material_Kd, materialKd);

    GLint loc_material_Ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(loc_material_Ks, materialKs);

    GLint loc_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(loc_shininess, materialShininess);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int hasTexture = 0;

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        hasTexture = 1;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_1"), 0);
    }

    GLint loc_has_texture = glGetUniformLocation(shader->program, "hasTexture");
    glUniform1i(loc_has_texture, hasTexture);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++)
        data[i] = rand();

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
