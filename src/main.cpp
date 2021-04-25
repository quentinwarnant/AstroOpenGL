// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

using namespace glm;

// -----------------------
// ASTRO
// -----------------------
#include "Window/AWindow.h"
#include "GameFramework/AstroGame.h"


constexpr GLsizei c_windowSizeW = 2304;
constexpr GLsizei c_windowSizeH = 1224;


void ShutDown(AWindow* window, AstroGame* astroGame)
{
    delete astroGame;
    astroGame = nullptr;

    delete window;
    window = nullptr;
}


void MainLoop()
{

}

int main(void )
{
    //INIT
    AWindow* window = new AWindow(c_windowSizeW, c_windowSizeH, "Astro Voxel Game");
	window->Init();

    AstroGame* astroGame = new AstroGame(); 

    do
    {
        window->PreUpdate();
		window->Update();

        astroGame->ReadInput();
        astroGame->Update();
        astroGame->Render();

        // Swap buffers
		window->SwapBuffers();
		glfwPollEvents();

		window->PostUpdate();


    }
    while (!window->ShouldClose() );


    ShutDown(window, astroGame);

    return 0;
}