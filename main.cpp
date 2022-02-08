#include "main.h"
#include "cells.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int POPULATION = 20;

const int MAX_WINDOW_WIDTH = 700;
const int MAX_WINDOW_HEIGHT = 700;

#define SCALE_WINDOW_TO_POPULATION

#ifdef SCALE_WINDOW_TO_POPULATION
	const int WINDOW_WIDTH = POPULATION * (MAX_WINDOW_WIDTH/POPULATION);
	const int WINDOW_HEIGHT = POPULATION * (MAX_WINDOW_HEIGHT/POPULATION);
#else
	const int WINDOW_WIDTH = MAX_WINDOW_WIDTH;
	const int WINDOW_HEIGHT = MAX_WINDOW_HEIGHT;
#endif

int check_error = 0;




int main ( int argc, char** argv )
{
	if ( !init_SDL() ) { return -1; }	

	Cells cells( POPULATION );

	if ( check_error < 0 )
	{
		printf( "Process failed with code:%d\n", check_error );
		return check_error;
	}



//----------------------------------Auxiliary Variables-----------------------------------//

	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/POPULATION) , (WINDOW_HEIGHT/POPULATION) };
	Gridmap grid = { 0, POPULATION-1, 0, POPULATION-1 };
	
	bool initialised = false;	
	bool displayed = false;
	bool draw_grid = false;
	bool init_random = false;
	bool init_from_image = !init_random && true;

	char image_path[100] = { 0 };
	strcpy( image_path, "binary_16_8.bmp" );

	bool quit = false;
	SDL_Event event;


//------------------------------------Preperations---------------------------------------//

	srand(time(NULL));
	
	SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
	SDL_RenderPresent( renderer );


//------------------------------------Main Loop-------------------------------------------//

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}			
		}

		if ( !initialised )
		{
			if ( init_random )
			{
				cells.init_random();
				initialised = true;
			}
			else if ( init_from_image )
			{
				initialised = true;
				int ret = cells.init_from_image( image_path );
				printf( "Ret=%d\n", ret );
			}
			else
			{
				initialised = cells.init( grid, unit_rect, event );

				SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
				SDL_RenderPresent( renderer );
			}

			continue;			
		}
		else if ( !displayed )
		{   //TODO: Do this with SDL events instead.
			displayed = true;
			cells.render( grid, unit_rect );
			SDL_RenderPresent( renderer );
			printf( "Displaying initial condition for confirmation.\n" );
			system("pause");
		}

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );


		
		cells.update();
		cells.render( grid, unit_rect );

		if ( draw_grid ) { SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF ); }


		SDL_RenderPresent( renderer );
		SDL_Delay(250);
	}


	quit_SDL();
	return 0;
}