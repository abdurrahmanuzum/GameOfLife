#include "main.h"
#include "cells.h"


#define SCALE_WINDOW_TO_POPULATION
#define IMAGE_PATH "./hammerhead_40ppc.bmp"
#define GRID_SHOWN false


SDL_Window*	  window   = NULL;
SDL_Renderer* renderer = NULL;

const int POPULATION = 50;

const int MAX_WINDOW_WIDTH = 700;
const int MAX_WINDOW_HEIGHT = 700;


#ifdef SCALE_WINDOW_TO_POPULATION
	const int WINDOW_WIDTH	= POPULATION * (MAX_WINDOW_WIDTH/POPULATION);
	const int WINDOW_HEIGHT = POPULATION * (MAX_WINDOW_HEIGHT/POPULATION);
#else
	const int WINDOW_WIDTH	= MAX_WINDOW_WIDTH;
	const int WINDOW_HEIGHT = MAX_WINDOW_HEIGHT;
#endif

// Will remove this soon
int check_error = 0;





int main ( int argc, char** argv )
{
	if ( !init_SDL() ) { return -1; }	

	Cells cells( POPULATION );

	if ( check_error < 0 )
	{
		fprintf( stderr, "Process failed with code:%d\n", check_error );
		return check_error;
	}


	//----------------------------------Auxiliary Variables-----------------------------------//

	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/POPULATION) , (WINDOW_HEIGHT/POPULATION) };
	Gridmap grid = { 0, POPULATION-1, 0, POPULATION-1 };
	
	SDL_Event event;

	INIT_TYPE init_type = INIT_TYPE::IMAGE;

	// Control flags	
	bool quit = false;	


	//------------------------------------Preperations---------------------------------------//

	srand(time(NULL));
	
	SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
	SDL_RenderPresent( renderer );

	// init_type will be passed into the main function in the future as a string
	switch( init_type )
	{
		// All init functions return 0 at success.
		case INIT_TYPE::USER:
			quit = cells.init_by_user( grid, unit_rect );
		break;

		case INIT_TYPE::IMAGE:
			quit = cells.init_by_imag( IMAGE_PATH, 39, 0 );
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

	// Display what the initial conditions are set to
	if ( !quit )
	{
		printf( "Initial conditions are set as shown, press any key to continue.\n" );
		cells.render( grid, unit_rect );
		SDL_RenderPresent( renderer );

		// Wait for user input then get rid of it.
		scanf( "%*[^\n]%*c" ); 
	}



	//------------------------------------Main Loop-------------------------------------------//

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}			

			// Zoom/pan handlers will go here
		}		

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );
		
		cells.update();
		cells.render( grid, unit_rect );

		if ( GRID_SHOWN ) { SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF ); }


		SDL_RenderPresent( renderer );
		SDL_Delay(100);
	}


	quit_SDL();
	return 0;
}