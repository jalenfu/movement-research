/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "library.hpp"
#include "dot.hpp"
#include "ltimer.hpp"
#include "complex_player.hpp"
#include "simple_player.hpp"
#include "input_handler.hpp"
#include "platform.hpp"
#include "target.hpp"
#include "game_state.hpp"
#include "menu.hpp"

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			InputHandler inputHandler;
			GameStateManager gameState;
			Menu menu;

			ComplexPlayer complexPlayer;
			SimplePlayer simplePlayer;

			LTimer capTimer;

			// Add Battlefield-style platforms (centered)
			PlatformManager::addPlatform(45, 400, 150, 20);   // Left platform
			PlatformManager::addPlatform(245, 300, 150, 20);  // Center platform (higher)
			PlatformManager::addPlatform(445, 400, 150, 20);  // Right platform

			// Spawn initial target
			TargetManager::spawnRandomTarget();

			// Initialize controller
			if (inputHandler.initController())
			{
				printf("Controller initialized successfully!\n");
			}
			else
			{
				printf("No controller found or failed to initialize controller.\n");
			}

			//While application is running
			while( !quit )
			{
				capTimer.start();
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					// Always update input handler for all events
					inputHandler.handleInput(e);

					// Menu-specific input
					if (gameState.isInMenu()) {
						menu.handleInput(e, gameState);
					}
				}

				// Handle controller input for menu (outside of event loop for continuous input)
				if (gameState.isInMenu()) {
					menu.handleControllerInput(inputHandler, gameState);
				}

				// Check for pause button (escape key or controller start button)
				if (gameState.isInGameplay()) {
					bool pausePressed = inputHandler.isKeyPressed(SDLK_ESCAPE);
					
					// Check controller start button directly
					if (inputHandler.isControllerConnected()) {
						bool startButtonPressed = inputHandler.getControllerHandler().isButtonPressed(6);
						if (startButtonPressed) {
							printf("Start button pressed! Opening menu...\n");
						}
						pausePressed = pausePressed || startButtonPressed;
					}
					
					if (pausePressed) {
						gameState.toggleMenu();
					}
				}

				//Update game based on current state
				if (gameState.isInMenu()) {
					menu.render();
				} else {
					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					//Update and render appropriate player
					if (gameState.getState() == GameState::COMPLEX_PLAYER) {
						complexPlayer.handleEvent(inputHandler);
						complexPlayer.update(inputHandler);
						complexPlayer.render();
					} else if (gameState.getState() == GameState::SIMPLE_PLAYER) {
						simplePlayer.handleEvent(inputHandler);
						simplePlayer.update(inputHandler);
						simplePlayer.render();
					}
					
					// Render platforms
					SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF); // Gray color
					for (const Platform& platform : PlatformManager::getPlatforms()) {
						SDL_Rect platformRect = {platform.x, platform.y, platform.width, platform.height};
						SDL_RenderFillRect(gRenderer, &platformRect);
					}
					
					// Render targets
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); // Red color
					for (const Target& target : TargetManager::getTargets()) {
						if (target.active) {
							SDL_Rect targetRect = {target.x, target.y, target.width, target.height};
							SDL_RenderFillRect(gRenderer, &targetRect);
						}
					}

					//Update screen
					SDL_RenderPresent( gRenderer );
				}

				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
