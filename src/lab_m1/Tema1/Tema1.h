#pragma once

#include "components/simple_scene.h"
#include "Object.h"
#include <random>
#include <set>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderSquare(Square square); // patrat
        void RenderRectangle(Rectangle rectangle); // dreptunghi
        void RenderStar(Star star);
        void RenderDiamond(Diamond diamond);
        void RenderCanon(Canon canon);
        void RenderPoints(Points points);
        void RenderEnemy(Enemy enemy);
        void RenderEnemies();
        void RenderStars();
        void RenderTerrains(); // patratele verzi + dreptunghi
        void RenderInterface();

        void InitAllMeshes();
        void InitTerrain(); // patratele verzi + dreptunghi
        void InitInterface();
        void InitEnemies(); // lista enemy
        void InitStars(); // lista stelute
    protected:
        glm::vec3 red = glm::vec3(0.796f, 0.007f, 0.011f);
        glm::vec3 orange = glm::vec3(0.925f, 0.329f, 0.023f);
        glm::vec3 yellow = glm::vec3(0.964f, 0.866f, 0.031f);
        glm::vec3 green = glm::vec3(0.227f, 0.890f, 0.419f);
        glm::vec3 blue = glm::vec3(0.047f, 0.380f, 0.956f);
        glm::vec3 purple = glm::vec3(0.611f, 0, 0.952f);
        glm::vec3 pink = glm::vec3(0.921f, 0, 0.933f);
        glm::vec3 grey = glm::vec3(0.6f, 0.6f, 0.6f);
        glm::vec3 black = glm::vec3(0, 0, 0);

        glm::vec3 center = glm::vec3(0, 0, 0);
        float squareSide = 100;
        glm::mat3 modelMatrix;

        std::vector<Square> terrains; // patrate verzi
        std::unordered_map<int, Canon> canons_in_use;
        Rectangle border; // dreptunghi rosu

        std::vector<Canon> canons; // romburi
        std::vector<Square> outlines; // outline
        std::vector<Points> points; // points
        std::vector<Star> stars;

        std::vector<Square> lives; // patrate rosii
        int numberOfLives = 3;
        Points score; // scorul

        Enemies enemies;

        Mouse mouse;
        Canon canon;

        float render_enemy_time = 0;
        float render_stars_time = 0;
    };
}   // namespace m1