#pragma once

#include <glad/glad.h>

enum cell_type
{
	AIR,
	DIRT,
	SAND,
	WATER
};

struct cell
{
	cell_type type;
	float lifetime;
};

float cell_vertices[] = {
	1.0f,  1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f,  1.0f
};

unsigned int cell_indices[] = {
	0, 1, 3,
	1, 2, 3
};

unsigned int quadVBO, quadEBO, quadVAO;



void Cells_Initialize()
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cell_vertices), cell_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cell_indices), cell_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Cell_BindCells()
{
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
}

glm::vec3 GetCellColor(cell_type type)
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
