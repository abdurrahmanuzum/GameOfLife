#include "environment.h"
#include "cells.h"
#include "timer.h"


int main ( int argc, char** argv )
{
	//Container for environment settings with default initialisations

	Env env = { 500,						//Window width
				500,						//Wýndow height
				4,							//Zoom factor
				false,						//Show grid
				0xAAAAAAFF,					//Grid color
				50,							//Population (2500)
				"./gun_50ppc.bmp", 50, 0,	//Default image and its properties
				INIT_TYPE::IMAGE };			//Initialise from image

	//Get command line options
	get_options( argc, argv, &env );

	//Initialise SDL
	if ( !init_SDL( env ) ) { return -1; }	

	// Create 2d array of cells of provided size
	Cells cells( env.population );
	if ( cells.allocated < 0 ) { return -1; }


	//----------------------------------Auxiliary Variables-----------------------------------//

	// To keep track of pan/zoom setting.
	SDL_Rect unit_rect   = { 0, 0, (env.window_w/env.population) , (env.window_h/env.population) };
	SDL_Rect border_rect = { 0, 0, env.window_w, env.window_w };
	Map		 map		 = { 0, env.population, 0, env.population, unit_rect, border_rect };
	
	SDL_Event event;
	bool quit = false;	


	//------------------------------------Preperations---------------------------------------//

	srand(time(NULL));

	switch( env.init_type )
	{
		// All init functions return 0 at success.
		case INIT_TYPE::USER:
			quit = cells.init_by_user( env, map );
		break;

		case INIT_TYPE::IMAGE:
			quit = cells.init_by_imag( env.image_path, env.image_ppc, env.image_alive_color );
		break;

		case INIT_TYPE::FILE:
			printf( "Not yet.\n" );
		break;

		case INIT_TYPE::RANDOM:
			quit = cells.init_by_rand();
		break;

		default:
			fprintf( stderr, "Initialisation type is not passed or invalid.\n" );
			quit = true;
		break;
	}

	// Display what the initial conditions are set to. Will find a better way of doing this.
	if ( !quit )
	{
		printf( "Initial conditions are set as shown, press any key to continue.\n" );
		cells.render( map );
		SDL_RenderPresent( renderer );

		SDL_Delay(1000);

		system("pause");
	}



	//------------------------------------Main Loop-------------------------------------------//

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
				break;
			}			

			handle_pan_zoom( &env, &map, event );
		}		


		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );

		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderDrawRect( renderer, &(map.border_rect) );
		
		cells.update();
		cells.render( map );

		if ( env.grid_shown ) { SDL_DrawGrid( env, map ); }

		SDL_RenderPresent( renderer );
		SDL_Delay(100);
	}


	quit_SDL();
	return 0;
}