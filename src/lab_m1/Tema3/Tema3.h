#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema3/Utils.h"

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        //void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, glm::vec3 color = glm::vec3(1));
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
    protected:
        custom::Boats boats;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        std::vector<int> types;
        std::vector<glm::vec3> colors;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> directions;
        std::vector<float> angles;
        glm::vec3 directLightPosition;
        glm::vec3 lighthouse_color = glm::vec3(1);
        glm::vec3 lighthouse_position;
        glm::vec3 direction1;
        glm::vec3 direction2;
        float angle1 = 0.1;
        float angle2 = 0;
    };
}   // namespace m1
