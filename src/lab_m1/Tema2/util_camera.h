#pragma once

#include <iostream>
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <random>
#include <set>
#include <map>


namespace custom
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, 1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
            distanceToTarget = glm::length(center - position);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }

        void MoveUpward(float distance)
        {
            position += glm::normalize(up) * distance;
        }

        void MoveRight(float distance)
        {
            position += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
            forward = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };

    inline int SelectRandom(int minRange, int maxRange) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(minRange, maxRange);

        return dis(gen);
    }

    inline float SelectRandomFloat(int minRange, int maxRange)
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<> dis(minRange, maxRange);

        return dis(gen);
    }

    class Object
    {
    public:
        Object() : mesh(nullptr), modelMatrix(glm::mat4(1)), 
                   dimensions(glm::vec3(0.0f)), radius(0.0f), aabbMin(glm::vec3(0.0f)), aabbMax(glm::vec3(0.0f)),
                   translateCoord(glm::vec3(0.0f)), scaleCoord(glm::vec3(1.0f)), angle(0.0f) {}

        Object(Mesh* mesh, glm::vec3 positionCoord)
        {
            this->mesh = mesh;
            modelMatrix = glm::mat4(1);
            translateCoord = positionCoord;
            scaleCoord = glm::vec3(1.0f);
            angle = 0;
        }

        Object(Mesh* mesh, glm::vec3 positionCoord, glm::vec3 scaleCoord, glm::vec3 dimensions)
        {
            this->mesh = mesh;
            modelMatrix = glm::mat4(1);

            this->dimensions = dimensions * scaleCoord;
            this->radius = sqrt(pow(this->dimensions.x, 2) + pow(this->dimensions.z, 2)) / 2;
            this->aabbMin = glm::vec3(-(this->dimensions.x * 1.2f) / 2, 0, -(this->dimensions.z * 1.2f) / 2);
            this->aabbMax = glm::vec3((this->dimensions.x * 1.2f) / 2, (this->dimensions.y * 1.2f), (this->dimensions.z * 1.2f) / 2);

            this->translateCoord = positionCoord;
            this->scaleCoord = scaleCoord;
            this->angle = 0;
        }

        ~Object() {}

        virtual void PutObject(glm::vec3 translate, glm::vec3 scale, float rotate)
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, translate);
            modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, scale);
        }

        void PutObject2(glm::vec3 translate, glm::vec3 scale, float rotate)
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::scale(modelMatrix, scale);
            modelMatrix = glm::translate(modelMatrix, translate);
            modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
        }

        void PutObject(glm::vec3 translate, float rotate)
        {
            PutObject(translate, this->scaleCoord, rotate);
        }

        void PutObject(glm::vec3 translate, float rotate, float deformation)
        {
            PutObject(translate, this->scaleCoord * glm::vec3(deformation), rotate);
        }

        void PutObject()
        {
            PutObject(this->translateCoord, this->angle);
        }
    public:
        Mesh* mesh;
        glm::mat4 modelMatrix;

        glm::vec3 dimensions;
        float radius;
        glm::vec3 aabbMin;
        glm::vec3 aabbMax;

        glm::vec3 translateCoord;
        glm::vec3 scaleCoord;
        float angle;
    };

    class Building : public Object
    {
    public:
        Building() : Object() {}

        Building(Mesh* mesh, glm::vec3 positionCoord) : Object(mesh, positionCoord)
        {
            float minScale = 0.5f;
            float maxScale = 3;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(minScale, maxScale);

            scaleCoord = glm::vec3(dis(gen), dis(gen), dis(gen));
            dimensions = scaleCoord;

            this->aabbMin = glm::vec3(-(this->dimensions.x * 1.2f) / 2, 0, -(this->dimensions.z * 1.2f) / 2);
            this->aabbMax = glm::vec3((this->dimensions.x * 1.2f) / 2, (this->dimensions.y * 1.2f), (this->dimensions.z * 1.2f) / 2);

            color = glm::vec3(0.2 * SelectRandom(1, 4));
        }
    public:
        glm::vec3 color;
    };

    class Buildings
    {
    public:
        std::vector<Building> buildings;
    public:
        Buildings()
        {
            buildings.resize(8);
        }

        Buildings(Mesh* cladire)
        {
            buildings.resize(20);
            float minPos = -10;
            float maxPos = 10;

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_real_distribution<float> dis(minPos, maxPos);

            float minDistance = 1;
            std::set<std::pair<float, float>> positions;

            while (positions.size() < 20)
            {
                float posX = dis(gen);
                float posZ = dis(gen);

                if ((posX <= -2 || posX >= 2) && (posZ <= -2 || posZ >= 2))
                {
                    bool validPosition = true;

                    for (const auto& pos : positions) {
                        if (glm::distance(posX, pos.first) <= minDistance && glm::distance(posZ, pos.second) <= minDistance) {
                            validPosition = false;
                            break;
                        }
                    }

                    if (validPosition) {
                        positions.insert(std::make_pair(posX, posZ));
                    }
                }
            }

            int count = 0;
            for (auto it = positions.begin(); it != positions.end(); ++it)
            {
                buildings[count] = custom::Building(cladire, glm::vec3(it->first, 0.0f, it->second));
                count++;
            }
        }

        void PutBuildings()
        {
            for (int i = 0; i < 20; i++)
            {
                buildings[i].PutObject();
            }
        }
    };

    class Projectile : public Object
    {
    public:
        Projectile() : Object()
        {
            timer = 0;
        }

        Projectile(Mesh* mesh5, glm::vec3 initialPos, glm::vec3 scaleCoord)
            : Object(mesh5, initialPos, scaleCoord, glm::vec3(0.3f, 0.3f, 0.3f))
        {
            timer = 0;
            radius = this->dimensions.x / 2;
        }

        Projectile(Mesh* mesh5, glm::vec3 positionCoord, glm::vec3 scaleCoord, float angle)
            : Object(mesh5, positionCoord, scaleCoord, glm::vec3(0.3f, 0.3f, 0.3f))
        {
            timer = 0;
            this->angle = angle;
            radius = this->dimensions.x / 2;
        }

        void ThrowProjectile(float deltaTime)
        {
            this->translateCoord.x += 2 * deltaTime * sin(this->angle);
            this->translateCoord.z += 2 * deltaTime * cos(this->angle);
        }
    public:
        bool isThrown;
        float timer;
    };

    class Tank : public Object
    {
    public:
        Tank()
        {
            translateCoord = glm::vec3(0.0f);
            angle = 0.0f;

            tracks = Object();
            body = Object();
            turret = Object();
            gun = Object();

            cooldown = 0;
            hp = 3;
        }

        Tank(Mesh* mesh1, Mesh* mesh2, Mesh* mesh3, Mesh* mesh4, Mesh* mesh5, glm::vec3 initialPos)
        {
            translateCoord = initialPos;
            angle = 0.0f;
            tracks = Object(mesh1, glm::vec3(initialPos.x, 0.1f, initialPos.z),
                glm::vec3(0.2f), glm::vec3(2.7f, 5.1f, 1.0f));
            body = Object(mesh2, glm::vec3(initialPos.x, 0.11f, initialPos.z),
                glm::vec3(0.2f), glm::vec3(3.1f, 1.2f, 4.6f));
            turret = Object(mesh3, glm::vec3(initialPos.x, 0.25f, initialPos.z),
                glm::vec3(0.2f), glm::vec3(1.7f, 0.9f, 2.0f));
            gun = Object(mesh4, glm::vec3(initialPos.x, 0.24f, initialPos.z),
                glm::vec3(0.2f), glm::vec3(0.6f, 3.8f, 0.6f));

            projectileMesh = mesh5;

            cooldown = 0;
            hp = 3;
        }

        ~Tank() {}

        virtual void PutTank()
        {
            tracks.PutObject(glm::vec3(this->translateCoord.x, tracks.translateCoord.y, this->translateCoord.z), this->angle);
            body.PutObject(glm::vec3(this->translateCoord.x, body.translateCoord.y, this->translateCoord.z), this->angle);
            turret.PutObject(glm::vec3(this->translateCoord.x, turret.translateCoord.y, this->translateCoord.z), this->angle);
            gun.PutObject(glm::vec3(this->translateCoord.x, gun.translateCoord.y, this->translateCoord.z), this->angle);

            for (int i = 0; i < projectiles.size(); i++)
            {
                projectiles[i].PutObject();
            }
        }

        void TranslateTank(float deltaTime)
        {
            this->translateCoord.x += deltaTime * sin(this->angle);
            this->translateCoord.z += deltaTime * cos(this->angle);
        }

        void RotateTank(float deltaTime)
        {
            this->angle += deltaTime;

            if (this->angle >= RADIANS(360.0f))
            {
                this->angle -= RADIANS(360.0f);
            }
            else if (this->angle <= -RADIANS(360.0f))
            {
                this->angle += RADIANS(360.0f);
            }
        }

        virtual void RotateTurret(float deltaTime) {}

    public:
        Object tracks;
        Object body;
        Object turret;
        Object gun;
        Mesh* projectileMesh;
        std::vector<Projectile> projectiles;
        float cooldown;
        int hp;
    };

    class Player : public Tank
    {
    public:
        Player() : Tank() {}

        Player(Mesh* mesh1, Mesh* mesh2, Mesh* mesh3, Mesh* mesh4, Mesh* mesh5)
            : Tank(mesh1, mesh2, mesh3, mesh4, mesh5, glm::vec3(0.0f)) {}

        void PutTank() override
        {
            tracks.PutObject(glm::vec3(this->translateCoord.x, tracks.translateCoord.y, this->translateCoord.z), this->angle);
            body.PutObject(glm::vec3(this->translateCoord.x, body.translateCoord.y, this->translateCoord.z), this->angle);
            turret.PutObject(glm::vec3(this->translateCoord.x, turret.translateCoord.y, this->translateCoord.z), turret.angle);
            gun.PutObject(glm::vec3(this->translateCoord.x, gun.translateCoord.y, this->translateCoord.z), turret.angle);

            for (int i = 0; i < projectiles.size(); i++)
            {
                projectiles[i].PutObject();
            }
        }

        void RotateTurret(float deltaTime) override
        {
            turret.angle += deltaTime;

            if (turret.angle >= RADIANS(360.0f))
            {
                turret.angle -= RADIANS(360.0f);
            }
            else if (turret.angle <= -RADIANS(360.0f))
            {
                turret.angle += RADIANS(360.0f);
            }
        }

        void AddProjectile()
        {
            if (cooldown >= 2 || projectiles.size() == 0)
            {
                glm::vec3 translate = glm::vec3(this->translateCoord.x, 0.24f, this->translateCoord.z);
                Projectile projectile = Projectile(projectileMesh, translate, glm::vec3(0.2f), turret.angle);
                projectiles.push_back(projectile);
                cooldown = 0;
            }
        }

        void ThrowProjectiles(float deltaTime)
        {
            for (int i = 0; i < projectiles.size(); i++)
            {
                projectiles[i].timer += deltaTime;
                projectiles[i].ThrowProjectile(-deltaTime);
            }
        }
    };

    enum Move
    {
        Forward,
        Backward,
        Left,
        Right
    };

    class Enemy : public Tank
    {
    public:
        Enemy() : Tank()
        {
            moveTime = 0;
            firstMove = Move::Forward;
        }

        Enemy(Mesh* mesh1, Mesh* mesh2, Mesh* mesh3, Mesh* mesh4, Mesh* mesh5, glm::vec3 initialPos)
            : Tank(mesh1, mesh2, mesh3, mesh4, mesh5, initialPos)
        {
            moveTime = 0;
            firstMove = Move::Forward;
        }

        float SelectTime()
        {
            float values[] = { 1.0f, 1.5f, 2.0f, 2.5f };

            int pos = SelectRandom(0, 3);

            return values[pos];
        }

        void MakeMove(const Move move, float deltaTime)
        {
            switch (move)
            {
            case Move::Forward:
                TranslateTank(-deltaTime);
                break;
            case Move::Backward:
                TranslateTank(deltaTime);
                break;
            case Move::Left:
                RotateTank(deltaTime);
                break;
            case Move::Right:
                RotateTank(-deltaTime);
                break;
            default:
                break;
            }
        }

        Move SelectMove(const Move currMove)
        {
            std::map<Move, std::vector<Move>> possibleMoves =
            {
                {Move::Forward, {Move::Left, Move::Right, Move::Backward}},
                {Move::Backward, {Move::Left, Move::Right, Move::Forward}},
                {Move::Left, {Move::Forward, Move::Backward, Move::Right}},
                {Move::Right, {Move::Forward, Move::Backward, Move::Left}}
            };

            std::vector<Move> nextMoves = possibleMoves[currMove];
            int randomIndex = SelectRandom(0, nextMoves.size() - 1);
            Move nextMove = nextMoves[randomIndex];

            return nextMove;
        }

        void MoveEnemy(float firstTime, float deltaTime)
        {
            MakeMove(firstMove, deltaTime);
            moveTime += deltaTime;

            if (moveTime >= firstTime)
            {
                moveTime = 0;

                firstMove = SelectMove(firstMove);
                MoveEnemy(SelectTime(), deltaTime);
            }
        }
    public:
        float moveTime;
        Move firstMove;
        float firstTime;
    };

    inline bool CollisionTankTank(Tank tank1, Tank tank2) 
    {
        glm::vec3 center1 = tank1.translateCoord;
        glm::vec3 center2 = tank2.translateCoord;
        glm::vec3 dif = glm::abs(glm::vec3(center2.x, 0, center2.z) - glm::vec3(center1.x, 0, center1.z));
        float distance = glm::length(dif);

        return distance <= tank1.tracks.radius + tank2.tracks.radius;
    }

    inline bool CollisionTankBuilding(Tank tank, Building building)
    {
        glm::vec3 center1 = tank.translateCoord;
        glm::vec3 center2 = building.translateCoord;
        glm::vec3 min = center2 + building.aabbMin - glm::vec3(0.2f);
        glm::vec3 max = center2 + building.aabbMax + glm::vec3(0.2f);

        return center1.x >= min.x && center1.x <= max.x && center1.z >= min.z && center1.z <= max.z;
    }

    inline bool CollisionTankProjectile(Tank tank, Projectile projectile) 
    {
        glm::vec3 center1 = tank.translateCoord;
        glm::vec3 center2 = projectile.translateCoord;
        glm::vec3 dif = glm::abs(glm::vec3(center2.x, 0, center2.z) - glm::vec3(center1.x, 0, center1.z));
        float distance = glm::length(dif);

        return distance <= tank.turret.radius + projectile.radius;
    }

    inline bool CollisionBuildingProjectile(Building building, Projectile projectile) 
    {
        glm::vec3 center1 = projectile.translateCoord;
        glm::vec3 center2 = building.translateCoord;
        glm::vec3 min = center2 + building.aabbMin + glm::vec3(0.2f);
        glm::vec3 max = center2 + building.aabbMax - glm::vec3(0.2f);

        return center1.x >= min.x && center1.x <= max.x && center1.z >= min.z && center1.z <= max.z;
    }

    inline void ShowDistanceTankTank(Tank tank1, Tank tank2)
    {
        glm::vec3 center1 = tank1.translateCoord;
        glm::vec3 center2 = tank2.translateCoord;
        glm::vec3 dif = glm::abs(glm::vec3(center2.x, 0, center2.z) - glm::vec3(center1.x, 0, center1.z));
        float distance = glm::length(dif);

        if (distance < 2)
        {
            //std::cout << "Pot trage in player\n";
        }
    }
}