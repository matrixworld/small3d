/*
*  GameLogic.h
*
*  Created on: 2014/11/09
*      Author: Dimitri Kourkoulis
*              http://dimitros.be
*     License: MIT
*/

#pragma once

#include <memory>
#include <dimitrikourk/small3d/Logger.h>
#include <dimitrikourk/small3d/SceneObject.h>
#include <dimitrikourk/small3d/Renderer.h>

#include <dimitrikourk/small3d/samplegame/KeyInput.h>

using namespace small3d;

namespace AvoidTheBug3D {

	class GameLogic {

	private:

		shared_ptr<SceneObject> goat;
		shared_ptr<SceneObject> bug;
		shared_ptr<SceneObject> tree;

		shared_ptr<Renderer> renderer;

		enum GameState {START_SCREEN, PLAYING};
		GameState gameState;

		enum BugState {FLYING_STRAIGHT, TURNING, DIVING_DOWN, DIVING_UP};
		BugState bugState, bugPreviousState;
		int bugFramesInCurrentState;
		float bugVerticalSpeed;

		void initGame();
		void processGame(const KeyInput &keyInput);
		void processStartScreen(const KeyInput &keyInput);

		void moveGoat(const KeyInput &keyInput);
		void moveBug();
		
	public:

		/**
		 * Constructor
		 */
		GameLogic();

		/**
		* Destructor
		*/
		~GameLogic();

		/**
		 * Process conditions and set up the next frame, also taking into consideration
		 * the input from the keyboard
		 * 
		 * @param	keyInput	The keyboard input
		 */
		void process(const KeyInput &keyInput);

		/**
		 * @fn	void GameLogic::render();
		 *
		 * @brief	Renders the current state of the game on the screen.
		 *
		 */

		void render();
	};

} /* namespace AvoidTheBug3D */

