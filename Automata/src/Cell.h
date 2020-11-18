#pragma once

#include <glad/glad.h>

#define CELL_COUNT_X 10
#define CELL_COUNT_Y 10

unsigned int CellTexture;

enum CellType
{
	AIR,
	DIRT,
	SAND,
	WATER
};

struct Cell
{
	CellType type;
};

Cell CellGrid[CELL_COUNT_X][CELL_COUNT_Y];

void Cell_Initialize()
{
	glGenTextures(1, &CellTexture);
}

void Cell_UpdateSandCell(int x, int y)
{

}

void Cell_UpdateWaterCell(int x, int y)
{

}

void Cell_UpdateDirtCell(int x, int y)
{

}

glm::vec3 Cell_GetCellColor(CellType type)
{
	switch (type)
	{
	case AIR: return glm::vec3(0.95f, 0.95f, 1.0f);
	case DIRT: return glm::vec3(0.415f, 0.341f, 0.023f);
	case SAND:return glm::vec3(0.937f, 0.839f, 0.423f);
	default:
	case WATER: return glm::vec3(0.070f, 0.698f, 0.870f);
	}
}

void Cell_GenerateCellTexture(int texture_unit)
{
	float pixels[] = {
	0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	};

	/*
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			glm::vec3 Color = Cell_GetCellColor(CellGrid[x][y].type);
			pixels[x + (y * CELL_COUNT_X) * 3] = Color.x;
			pixels[(x + (y * CELL_COUNT_X) * 3) + 1] = Color.y;
			pixels[(x + (y * CELL_COUNT_X) * 3) + 2] = Color.z;
		}
	}
	*/

	//glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, CellTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
}
