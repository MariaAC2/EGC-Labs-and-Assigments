#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <utility>
#include <iostream>

#include "Transform.h"
#include "Drawing.h"

namespace m1
{
	void Tema1::InitTerrain()
	{
		// teren
		Mesh* terrain = drawing::CreateSquare("terrain", center, squareSide, green, true);
		AddMeshToList(terrain);

		float posX = 150, posY = 100;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				terrains.push_back(Square::Square(terrain, green, std::make_pair(posX, posY)));
				posX += 150;
			}

			posX = 150;
			posY += 150;
		}

		Mesh* rectangle = drawing::CreateRectangle("rectangle", center, squareSide, red);
		AddMeshToList(rectangle);

		border = Rectangle::Rectangle(rectangle, red, std::make_pair(50, 250));
	}

	void Tema1::InitInterface()
	{
		// chenar
		Mesh* outline = drawing::CreateSquare("outline", center, squareSide, black, false);
		AddMeshToList(outline);

		// stelute scor
		Mesh* point = drawing::CreateStar("point", center, squareSide, grey);
		AddMeshToList(point);

		// toate romburile
		Mesh* orange_diamond_1 = drawing::CreateDiamond("orange_diamond_1", center, squareSide, orange);
		AddMeshToList(orange_diamond_1);

		Mesh* blue_diamond_1 = drawing::CreateDiamond("blue_diamond_1", center, squareSide, blue);
		AddMeshToList(blue_diamond_1);

		Mesh* yellow_diamond_1 = drawing::CreateDiamond("yellow_diamond_1", center, squareSide, yellow);
		AddMeshToList(yellow_diamond_1);

		Mesh* purple_diamond_1 = drawing::CreateDiamond("purple_diamond_1", center, squareSide, purple);
		AddMeshToList(purple_diamond_1);

		// toate stelutele
		Mesh* orange_star = drawing::CreateStar("orange_star", center, squareSide, orange);
		AddMeshToList(orange_star);

		Mesh* blue_star = drawing::CreateStar("blue_star", center, squareSide, blue);
		AddMeshToList(blue_star);

		Mesh* yellow_star = drawing::CreateStar("yellow_star", center, squareSide, yellow);
		AddMeshToList(yellow_star);

		Mesh* purple_star = drawing::CreateStar("purple_star", center, squareSide, purple);
		AddMeshToList(purple_star);

		canon = Canon::Canon();

		// pentru vieti
		Mesh* life = drawing::CreateSquare("life", center, squareSide, red, true);
		AddMeshToList(life);

		float posX = 150, posY = 650;

		outlines.resize(4);
		canons.resize(4);
		points.resize(4);

		outlines[0] = Square::Square(outline, black, std::make_pair(posX, posY));
		canons[0] = Canon::Canon(orange_diamond_1, orange_star, orange, std::make_pair(posX, posY), 1);
		points[0] = Points::Points(point, grey, std::make_pair(posX, posY), 1);

		posX += 150;

		outlines[1] = Square::Square(outline, black, std::make_pair(posX, posY));
		canons[1] = Canon::Canon(blue_diamond_1, blue_star, blue, std::make_pair(posX, posY), 2);
		points[1] = Points::Points(point, grey, std::make_pair(posX, posY), 2);

		posX += 150;

		outlines[2] = Square::Square(outline, black, std::make_pair(posX, posY));
		canons[2] = Canon::Canon(yellow_diamond_1, yellow_star, yellow, std::make_pair(posX, posY), 2);
		points[2] = Points::Points(point, grey, std::make_pair(posX, posY), 2);

		posX += 150;

		outlines[3] = Square::Square(outline, black, std::make_pair(posX, posY));
		canons[3] = Canon::Canon(purple_diamond_1, purple_star, purple, std::make_pair(posX, posY), 3);
		points[3] = Points::Points(point, grey, std::make_pair(posX, posY), 3);

		posX += 200;

		lives.resize(3);
		score = Points::Points(point, grey, std::make_pair(posX + 15, posY), 3);

		for (int i = 0; i < 3; i++)
		{
			lives[i] = Square::Square(life, red, std::make_pair(posX, posY));

			if (i < 2)
			{
				posX += 125;
			}
		}
	}

	void Tema1::InitEnemies()
	{
		enemies = Enemies::Enemies();

		Mesh* orange_hexagon = drawing::CreateHexagon("orange_hexagon", center, squareSide, orange);
		AddMeshToList(orange_hexagon);
		enemies.meshes.push_back(orange_hexagon);
		enemies.colors.push_back(orange);

		Mesh* blue_hexagon = drawing::CreateHexagon("blue_hexagon", center, squareSide, blue);
		AddMeshToList(blue_hexagon);
		enemies.meshes.push_back(blue_hexagon);
		enemies.colors.push_back(blue);

		Mesh* yellow_hexagon = drawing::CreateHexagon("yellow_hexagon", center, squareSide, yellow);
		AddMeshToList(yellow_hexagon);
		enemies.meshes.push_back(yellow_hexagon);
		enemies.colors.push_back(yellow);

		Mesh* purple_hexagon = drawing::CreateHexagon("purple_hexagon", center, squareSide, purple);
		AddMeshToList(purple_hexagon);
		enemies.meshes.push_back(purple_hexagon);
		enemies.colors.push_back(purple);

		enemies.posY.push_back(100);
		enemies.posY.push_back(250);
		enemies.posY.push_back(400);
	}

	void Tema1::InitStars()
	{
		Mesh* pink_star = drawing::CreateStar("pink_star", center, squareSide, pink);
		AddMeshToList(pink_star);

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> disX(200.0, 1000.0);
		std::uniform_real_distribution<float> disY(200.0, 500.0);

		stars.resize(3);

		std::set<std::pair<float, float>> positions;

		float minDistance = 100.0f;

		while (positions.size() < 3) 
		{
			float posX = disX(gen);
			float posY = disY(gen);

			bool validPosition = true;

			// Check if the generated position is at least minDistance away from existing positions
			for (const auto& pos : positions) {
				if (glm::distance(posX, pos.first) <= minDistance && glm::distance(posY, pos.second) <= minDistance) {
					validPosition = false;
					break;
				}
			}

			if (validPosition) {
				positions.insert(std::make_pair(posX, posY));
			}
		}

		int count = 0;
		for (auto it = positions.begin(); it != positions.end(); ++it) {
			stars[count] = Star::Star(pink_star, pink, std::make_pair(it->first, it->second));
			count++;
		}
	}
}