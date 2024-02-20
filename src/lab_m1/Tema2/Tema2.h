#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/util_camera.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        custom::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        glm::vec3 translatePos = glm::vec3(0, 0, 0);
        float angle = RADIANS(0.0f);

        custom::Player player;

        float firstTime = 0;

        custom::Buildings buildings;

        custom::Object terrain;

        std::vector<custom::Enemy> enemies;
    };
}   // namespace m1
