#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <utility>
#include <iostream>

#include "Transform.h"
#include "Drawing.h"

namespace m1
{
	void Tema1::RenderSquare(Square square)
	{
		square.PutObject();
		RenderMesh2D(square.mesh, shaders["VertexColor"], square.modelMatrix);
	}

	void Tema1::RenderRectangle(Rectangle rectangle)
	{
		rectangle.PutObject();
		RenderMesh2D(rectangle.mesh, shaders["VertexColor"], rectangle.modelMatrix);
	}

	void Tema1::RenderTerrains()
	{
		RenderRectangle(border);

		for (int i = 0; i < terrains.size(); i++)
		{
			RenderSquare(terrains[i]);
		}
	}

	void Tema1::RenderStar(Star star)
	{
		star.PutObject();
		RenderMesh2D(star.mesh, shaders["VertexColor"], star.modelMatrix);
	}

	void Tema1::RenderStars()
	{
		for (int i = 0; i < 3; i++)
		{
			if (!stars[i].isClicked)
			{
				RenderStar(stars[i]);
			}
		}
	}

	void Tema1::RenderPoints(Points points)
	{
		for (Star& point : points.points)
		{
			point.PutObject();
			point.modelMatrix *= transform2D::Scale(0.4f, 0.4f);
			RenderMesh2D(point.mesh, shaders["VertexColor"], point.modelMatrix);
		}
	}

	void Tema1::RenderDiamond(Diamond diamond)
	{
		diamond.PutObject();
		RenderMesh2D(diamond.mesh, shaders["VertexColor"], diamond.modelMatrix);
	}

	void Tema1::RenderCanon(Canon canon)
	{
		RenderDiamond(canon.canon);
	}

	void Tema1::RenderInterface()
	{

		for (int i = 0; i < 4; i++)
		{
			RenderSquare(outlines[i]);
			RenderCanon(canons[i]);
			RenderPoints(points[i]);
		}

		for (int i = 0; i < 3; i++)
		{
			lives[i].PutObject();
			lives[i].modelMatrix *= transform2D::Scale(0.75f, 0.75f);
			RenderMesh2D(lives[i].mesh, shaders["VertexColor"], lives[i].modelMatrix);
		}

		RenderPoints(score);
	}
}