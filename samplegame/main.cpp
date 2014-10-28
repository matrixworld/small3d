#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <dimitrikourk/small3d/EngineLog.h>
#include <dimitrikourk/small3d/EngineException.h>
#include <memory>

#include <dimitrikourk/small3d/samplegame/GameLogic.h>
#include <dimitrikourk/small3d/samplegame/PlayerView.h>
#include <dimitrikourk/small3d/samplegame/KeyInput.h>

using namespace std;
using namespace AvoidTheBug3D;
using namespace small3d;

const GLuint frameRate = 60;

KeyInput input;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        input.down = true;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        input.up = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        input.left = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        input.right = true;
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        input.enter = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        input.down = false;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        input.up = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        input.left = false;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        input.right = false;
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
        input.enter = false;
}

int main(int argc, char** argv)
{
	shared_ptr<EngineLog> log(new EngineLog(cout));

	shared_ptr<Configuration> cfg(new Configuration(log));

	try
	{

		shared_ptr<GameLogic> gameLogic(new GameLogic(cfg, log));

		shared_ptr<PlayerView> pv(new PlayerView(cfg, log));

		// program main loop
		bool done = false;

		// seconds, for setting the framerate
		double seconds = glfwGetTime();
		double prevSeconds = seconds;
		double secondsInterval = 1.0 / frameRate;
        
        glfwSetKeyCallback(pv->renderer->window, keyCallback);

		while (!glfwWindowShouldClose(pv->renderer->window))
		{

            glfwPollEvents();
            
			seconds = glfwGetTime();
			if (seconds - prevSeconds > secondsInterval)
			{
				gameLogic->process(input);
				prevSeconds = seconds;
				pv->render(gameLogic->gameScene);
			}
		}

	}
	catch (EngineException &e)
	{
		LOGERROR(e.what());
		return 1;
	}

	return 0;
}
