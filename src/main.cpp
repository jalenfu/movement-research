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
#include "player.hpp"
#include "input_handler.hpp"

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
			InputHandler keyboardHandler;

			Player player;

			LTimer capTimer;

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

					//Handle input for the dot
					/*
						Room for centralized input/event handler. Would handle all inputs? Would also handle all general updates below on line 64
					*/
					keyboardHandler.handleInput(e);
				}

				//Move the dot
				player.handleEvent(keyboardHandler);
				player.update();

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				player.render();

				//Update screen
				SDL_RenderPresent( gRenderer );

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
