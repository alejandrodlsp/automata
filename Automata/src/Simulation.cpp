#include <iostream>

#include "Window.h"
#include "RenderQuad.h"
#include "Cell.h"

#define WINDOW_SIZEX 900
#define WINDOW_SIZEY 900
#define FRAME_RATE 0.002

Window* window;

float lastFrame = 0.0;
float deltaTime = 0.0;
float frameElapsed = FRAME_RATE;

void UpdateInput();

CellType brush = WATER; 

int main()
{
	window = new Window();
	window->Init({ WINDOW_SIZEX, WINDOW_SIZEY, "Cellular automata" });

	RenderQuad quad;
	Cell_Initialize();

	
	Cell_GenerateCellTexture(GL_TEXTURE0);

	bool shouldClose = false;
	while (!shouldClose)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameElapsed += deltaTime;

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		UpdateInput();

		if (frameElapsed * 1000 > FRAME_RATE)
		{
			frameElapsed = 0;
			
			Cell_UpdateCells();
			Cell_GenerateCellTexture(GL_TEXTURE0);
			quad.Render(GL_TEXTURE0);

			window->Update();
		}

		shouldClose = window->ShouldClose();
	}

	window->Shutdown();
}

void UpdateInput()
{
	int brush_size = 2;
	if (glfwGetMouseButton(window->GetNative(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window->GetNative(), &xpos, &ypos);
		xpos /= WINDOW_SIZEX;	// Normalize
		ypos /= WINDOW_SIZEY;
		xpos = (xpos < 0) ? 0.0 : xpos; // Clamp
		ypos = (ypos < 0) ? 0.0 : ypos;
		int xcell = floor(xpos * CELL_COUNT_X);
		int ycell = floor(ypos * CELL_COUNT_Y);

		for (int x = -brush_size; x < brush_size; x++)
		{
			for (int y = -brush_size; y < brush_size; y++)
			{
				if (xcell + x >= 0 && xcell + x < CELL_COUNT_X)
					if (ycell + y >= 0 && ycell + y < CELL_COUNT_Y)
						CellGrid[xcell + x][CELL_COUNT_Y - (ycell + y + 1)].type = brush;
			}
		}
	}

	if (glfwGetKey(window->GetNative(), GLFW_KEY_S) == GLFW_PRESS) brush = SAND;
	if (glfwGetKey(window->GetNative(), GLFW_KEY_D) == GLFW_PRESS) brush = DIRT;
	if (glfwGetKey(window->GetNative(), GLFW_KEY_W) == GLFW_PRESS) brush = WATER;
}