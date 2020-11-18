// Automata.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Cell.h"
#include "Shader.h"

#define WINDOW_SIZEX 600
#define WINDOW_SIZEY 600

#define CELL_SIZE 10
#define FRAME_TIME 20

constexpr int cell_count_y{ WINDOW_SIZEY / CELL_SIZE };
constexpr int cell_count_x{ WINDOW_SIZEX / CELL_SIZE };

cell cell_grid[cell_count_y][cell_count_x];

float lastFrame = 0.0;
float deltaTime = 0.0;
float frameElapsed = FRAME_TIME;

Shader* cellShader;
Window* window;

bool shouldClose = false;

void UpdateInput();
void UpdateFrame();
void RenderFrame();
void UpdateSand(int x, int y);
void UpdateDirt(int x, int y);
void UpdateWater(int x, int y);

int main1()
{
	window = new Window();
	window->Init( { WINDOW_SIZEX, WINDOW_SIZEY, "Cellular automata" } );

	cellShader = new Shader("res/shader/cell.vs", "res/shader/cell.fs");
	cellShader->Bind();

	Cells_Initialize();
	while (!shouldClose) 
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameElapsed += deltaTime;
		
		UpdateInput();

		if (frameElapsed * 1000 >= FRAME_TIME)
		{
			frameElapsed = 0;
			UpdateFrame();
			RenderFrame();
		}

		shouldClose = window->ShouldClose();
	}
	return 0;
}

void UpdateInput()
{
	cell_type brush = SAND;
	int brush_size = 2;
	if (glfwGetMouseButton(window->GetNative(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window->GetNative(), &xpos, &ypos);
		xpos /= WINDOW_SIZEX;	// Normalize
		ypos /= WINDOW_SIZEY;
		xpos = (xpos < 0) ? 0.0 : xpos; // Clamp
		ypos = (ypos < 0) ? 0.0 : ypos;
		int xcell = floor(xpos * cell_count_x);
		int ycell = floor(ypos * cell_count_y);
		
		for (int x = -brush_size; x < brush_size; x++)
		{
			for (int y = -brush_size; y < brush_size; y++)
			{
				if (xcell + x >= 0 && xcell + x < cell_count_x)
					if (ycell + y >= 0 && ycell + y < cell_count_y)
						cell_grid[xcell + x][cell_count_y - ycell + y].type = brush;
			}
		}
	}
}

void UpdateFrame()
{
	for (int y = 0; y < cell_count_y; y++)
	{
		for (int x = 0; x < cell_count_x; x++)
		{
			switch(cell_grid[x][y].type)
			{
			case AIR: break;
			case SAND: UpdateSand(x, y);
			case DIRT: UpdateDirt(x, y);
			case WATER: UpdateWater(x, y);
			}
		}
	}
}

void RenderFrame()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	cellShader->Bind();
	Cell_BindCells();

	for (int x = 0; x < cell_count_x; x++)
	{
		for (int y = 0; y < cell_count_y; y++)
		{
			if (cell_grid[x][y].type == AIR) continue;

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 scale = glm::vec3(2.0f / cell_count_x, 2.0f / cell_count_y, 0.5f);
			model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(x * scale.x, y * scale.y, 0.0f));
			model = glm::scale(model, scale);
			cellShader->Bind();
			cellShader->SetUniformVec3("uColor", GetCellColor(cell_grid[x][y].type));
			cellShader->SetUniformMat4("uModel", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	window->Update();
}

void UpdateSand(int x, int y)
{
	// rows = y;
	// cols = x;
	if (y - 1 < 0) return;
	if (cell_grid[x][y - 1].type == AIR)
	{
		cell_grid[x][y].type = AIR;
		cell_grid[x][y - 1].type = SAND;
	}
	else if (x - 1 >= 0 && cell_grid[x - 1][y-1].type == AIR)
	{
		cell_grid[x][y].type = AIR;
		cell_grid[x-1][y-1].type = SAND;
	}
	else if (x + 1 < cell_count_x && cell_grid[x + 1][y-1].type == AIR)
	{
		cell_grid[x][y].type = AIR;
		cell_grid[x + 1][y - 1].type = SAND;
	}
}

void UpdateDirt(int x, int y)
{

}

void UpdateWater(int x, int y)
{

}