/*
 *  main.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *              http://dimitros.be
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <dimitrikourk/small3d/Exception.hpp>
#include <memory>

#include <dimitrikourk/small3d/samplegame/GameLogic.hpp>
#include <dimitrikourk/small3d/samplegame/KeyInput.hpp>

using namespace std;
using namespace AvoidTheBug3D;
using namespace small3d;

const GLuint frameRate = 60;

int main(int argc, char** argv)
{
  // Set up a console, if using MinGW
  // This is because the mwindows linker flag,
  // used by blocks referenced by small3d,
  // eliminates the default one.
#ifdef __MINGW32__
  AllocConsole();
  freopen("CONOUT$", "w", stdout);
#endif

  KeyInput input;

  try
  {

    shared_ptr<GameLogic> gameLogic(new GameLogic());

    // program main loop
    bool done = false;

    // ticks for setting the frame rate
    GLuint ticks = SDL_GetTicks();
    GLuint prevTicks = ticks;
    GLuint ticksInterval = 1000 / frameRate;

    while (!done)
    {

      SDL_Event event;
      if (SDL_PollEvent(&event))
      {

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        input.up = keyState[SDL_SCANCODE_UP] == 1;
        input.down = keyState[SDL_SCANCODE_DOWN] == 1;
        input.left = keyState[SDL_SCANCODE_LEFT] == 1;
        input.right = keyState[SDL_SCANCODE_RIGHT] == 1;
        input.enter = keyState[SDL_SCANCODE_RETURN] == 1;

        switch (event.type)
        {

        case SDL_QUIT:
          done = true;
          break;

        case SDL_KEYDOWN:
        {
          if (event.key.keysym.sym == SDLK_ESCAPE)
            done = true;
          break;
        }
        }
      }

      ticks = SDL_GetTicks();
      if (ticks - prevTicks > ticksInterval)
      {
        gameLogic->process(input);
        prevTicks = ticks;
        gameLogic->render();
      }
    }

  }
  catch (Exception &e)
  {
    LOGERROR(e.what());
    return 1;
  }

  return 0;
}
