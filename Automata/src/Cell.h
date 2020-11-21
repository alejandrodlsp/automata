#pragma once

#include <glad/glad.h>

#define CELL_COUNT_X 150
#define CELL_COUNT_Y 150

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
	bool updated = false;
};

Cell CellGrid[CELL_COUNT_X][CELL_COUNT_Y];

void Cell_Initialize()
{
	glGenTextures(1, &CellTexture);
	glBindTexture(GL_TEXTURE_2D, CellTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Cell_UpdateSandCell(int x, int y)
{
	if (y - 1 < 0) return;
	if (CellGrid[x][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x][y - 1].type = SAND;
	}
	else if (x - 1 >= 0 && CellGrid[x - 1][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x - 1][y - 1].type = SAND;
	}
	else if (x + 1 < CELL_COUNT_X && CellGrid[x + 1][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x + 1][y - 1].type = SAND;
	}
	else if (CellGrid[x][y - 1].type == WATER)
	{
		CellGrid[x][y].type = WATER;
		CellGrid[x][y - 1].type = SAND;
	}
	else if (x - 1 >= 0 && CellGrid[x - 1][y - 1].type == WATER)
	{
		CellGrid[x][y].type = WATER;
		CellGrid[x - 1][y - 1].type = SAND;
	}
	else if (x + 1 < CELL_COUNT_X && CellGrid[x + 1][y - 1].type == WATER)
	{
		CellGrid[x][y].type = WATER;
		CellGrid[x + 1][y - 1].type = SAND;
	}
}

void Cell_UpdateWaterCell(int x, int y)
{
	if (y - 1 >= 0 && CellGrid[x][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x][y - 1].type = WATER;
	}
	else if (y - 1 >= 0 && x - 1 >= 0 && CellGrid[x - 1][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x - 1][y - 1].type = WATER;
	}
	else if (y - 1 >= 0 &&x + 1 < CELL_COUNT_X && CellGrid[x + 1][y - 1].type == AIR)
	{
		CellGrid[x][y].type = AIR;
		CellGrid[x + 1][y - 1].type = WATER;
	}
	else
	{
		if (x - 1 >= 0 && CellGrid[x - 1][y].type == AIR)
		{
			CellGrid[x][y].type = AIR;
			CellGrid[x - 1][y].type = WATER;
		}
		else if (x + 1 < CELL_COUNT_X && CellGrid[x + 1][y].type == AIR)
		{
			CellGrid[x][y].type = AIR;
			CellGrid[x + 1][y].type = WATER;
		}
	}
}

void Cell_UpdateCells()
{
	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			CellGrid[x][y].updated = false;
		}
	}

	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = CELL_COUNT_X - 1; x >= 0; x--)
		{
			if (CellGrid[x][y].updated == false){
				CellGrid[x][y].updated = true;
				switch (CellGrid[x][y].type)
				{
				case AIR: break;
				case SAND: Cell_UpdateSandCell(x, y);
				case DIRT: break;
				case WATER:Cell_UpdateWaterCell(x, y); break;
				}
			}
		}
	}
}

glm::vec3 Cell_GetCellColor(CellType type)
{
	switch (type)
	{
	case WATER: return glm::vec3(0.419f, 0.764f, 0.980f);
	case SAND:return glm::vec3(0.992f, 0.886f, 0.749f);
	case DIRT: return glm::vec3(0.458f, 0.266f, 0.003f);
	default:
	case AIR: return glm::vec3(0.921f, 0.988f, 1.0f);
	}
}

void Cell_GenerateCellTexture(int texture_unit = GL_TEXTURE0)
{
	float* pixels = new float[(CELL_COUNT_X * CELL_COUNT_Y) * 3];

	for (int y = 0; y < CELL_COUNT_Y; y++)
	{
		for (int x = 0; x < CELL_COUNT_X; x++)
		{
			glm::vec3 Color = Cell_GetCellColor(CellGrid[x][y].type);
			pixels[(x + (y * CELL_COUNT_X)) * 3] = Color.x;
			pixels[((x + (y * CELL_COUNT_X)) * 3) + 1] = Color.y;
			pixels[((x + (y * CELL_COUNT_X)) * 3) + 2] = Color.z;
		}
	}

	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, CellTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CELL_COUNT_X, CELL_COUNT_Y, 0, GL_RGB, GL_FLOAT, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}
