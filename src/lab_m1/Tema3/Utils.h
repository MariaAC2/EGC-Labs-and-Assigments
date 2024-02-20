#pragma once

#include <iostream>
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/Tema2/util_camera.h"

namespace custom
{
	class Boat : public Object
	{
	public:
		Boat() : Object()
		{
			angle = 0;
			radius = 0;
			color = glm::vec3(0.0);
			orientation = -1;
		}

		Boat(float angle, float radius, glm::vec3 color, int orientation, float speed)
		{
			this->angle = angle;
			this->radius = radius;
			this->color = color;
			this->orientation = orientation;
			this->speed = speed;

			this->translateCoord = glm::vec3(radius * sin(angle), 0.15, radius * cos(angle));

		}

		void RotateBoat(float deltaTimeSeconds)
		{
			if (orientation == 0)
			{
				angle += speed * deltaTimeSeconds;

				if (angle >= RADIANS(360))
				{
					angle -= RADIANS(360);
				}
			}
			else
			{
				angle -= speed * deltaTimeSeconds;

				if (angle <= RADIANS(360))
				{
					angle += RADIANS(360);
				}
			}

			this->translateCoord = glm::vec3(radius * sin(angle), 0.15, radius * cos(angle));
		}

		void PutBoat()
		{
			base.PutObject2(this->translateCoord, glm::vec3(0.5), angle);
			light.PutObject2(this->translateCoord, glm::vec3(0.5), angle);

			if (orientation == 0)
			{
				tag.PutObject2(this->translateCoord, glm::vec3(0.5), angle);
			}
			else
			{
				tag.PutObject2(this->translateCoord, glm::vec3(0.5), angle + RADIANS(180));
			}
		}
	public:
		Object base;
		Object light;
		Object tag;
		glm::vec3 color;
		float radius;
		int orientation;
		int speed;
	};

	inline std::set<float> SelectNoRepeats(float minPos, float maxPos, float minDistance)
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dis(minPos, maxPos);

		std::set<float> positions;

		while (positions.size() < 4)
		{
			float pos = dis(gen);

			bool validPosition = true;
			for (const auto& position : positions) {
				if (glm::distance(pos, position) <= minDistance) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				positions.insert(pos);
			}
		}

		return positions;
	}

	class Boats
	{
	public:
		Boats()
		{
			boats.resize(4);

			std::set<float> radius_set = SelectNoRepeats(3, 10, 1);
			std::vector<float> radius_vector(radius_set.begin(), radius_set.end());
			std::set<float> angle_set = SelectNoRepeats(RADIANS(0), RADIANS(270), RADIANS(30));
			std::vector<float> angle_vector(angle_set.begin(), angle_set.end());

			for (int i = 0; i < 4; i++) {
				glm::vec3 color = glm::vec3(SelectRandom(0, 1), SelectRandom(0, 1), SelectRandom(0, 1));
				int orientation = SelectRandom(0, 1);
				float speed = SelectRandomFloat(1, 3.5);
				boats[i] = Boat(angle_vector[i], radius_vector[i], color, orientation, speed);
			}
		}

		void RotateBoats(float deltaTimeSeconds)
		{
			for (int i = 0; i < 4; i++)
			{
				boats[i].RotateBoat(deltaTimeSeconds);
			}
		}

		void PutBoats()
		{
			for (int i = 0; i < 4; i++)
			{
				boats[i].PutBoat();
			}
		}

	public:
		std::vector<Boat> boats;
	};

	struct Light
	{
		int type;
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 direction;
		float angle;
	};
}