#include <iostream>

#include "Window.h"
#include "RenderQuad.h"
#include "Cell.h"

#define WINDOW_SIZEX 600
#define WINDOW_SIZEY 600

Window* window;

int main()
{
	window = new Window();
	window->Init({ WINDOW_SIZEX, WINDOW_SIZEY, "Cellular automata" });

	RenderQuad quad;
	Cell_Initialize();

	bool shouldClose = false;
	while (!shouldClose)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Cell_GenerateCellTexture(GL_TEXTURE0);
		quad.Render(GL_TEXTURE0);

		window->Update();
		shouldClose = window->ShouldClose();
	}

	window->Shutdown();
}