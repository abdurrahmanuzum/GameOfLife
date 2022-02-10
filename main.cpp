#include "main.h"
#include "cells.h"


#define SCALE_WINDOW_TO_POPULATION
#define GRID_SHOWN false

#define IMAGE_PATH	 "./gun_50ppc.bmp" //Screen captured from Wikipedia heheh.
#define IMAGE_PPC	 50 //Pixels per cell on the source image
#define ACTIVE_COLOR 0  //Alive cell color on the source image, 1 for white, 0 for black


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

const int CELL_LENGTH_MIN = WINDOW_WIDTH / POPULATION;
const int CELL_LENGTH_MAX = WINDOW_WIDTH / 20;


// Will be carried to another file or a class that deals with display business, wrapping SDL as well
void handle_pan_zoom( Gridmap* grid, SDL_Event& event )
{
	static bool	  held   = false;
	static int	  prev_x = 0;
	static int	  prev_y = 0;
	int			  curr_x = 0;
	int			  curr_y = 0;
	static double diff_x = 0;
	static double diff_y = 0;

	// Zoom handling
	if ( event.type == SDL_MOUSEWHEEL ) 
	{
		grid->unit_rect.w += event.wheel.y;
		grid->unit_rect.h += event.wheel.y;

		if		( grid->unit_rect.w < CELL_LENGTH_MIN ) { grid->unit_rect.w = CELL_LENGTH_MIN; }
		else if ( grid->unit_rect.w > CELL_LENGTH_MAX ) { grid->unit_rect.w = CELL_LENGTH_MAX; }
		if		( grid->unit_rect.h < CELL_LENGTH_MIN ) { grid->unit_rect.h = CELL_LENGTH_MIN; }
		else if ( grid->unit_rect.h > CELL_LENGTH_MAX ) { grid->unit_rect.h = CELL_LENGTH_MAX; }
	}
	// Pan handling
	else if ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_MIDDLE )
	{
		held = true;
		SDL_GetMouseState( &prev_x, &prev_y );
	}
	else if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_MIDDLE )
	{
		held = false;

		// Clear the containers
		prev_x = 0;
		prev_y = 0;
		diff_x = 0;
		diff_y = 0;
	}

	if ( held )
	{
		SDL_GetMouseState( &curr_x, &curr_y );

		diff_x -= (double)(curr_x - prev_x)/grid->unit_rect.w;
		diff_y -= (double)(curr_y - prev_y)/grid->unit_rect.h;

		grid->x_index_first = diff_x;
		grid->y_index_first = diff_y;

		prev_x = curr_x;
		prev_y = curr_y;
	}

	// These are here and not in their respective if statement because they need to be updated if <>_index_first changes.
	// ORing the condition with held flag doesn't work and I don't know why.
	grid->x_index_last = ceil( (double)WINDOW_WIDTH /grid->unit_rect.w + grid->x_index_first );
	grid->y_index_last = ceil( (double)WINDOW_HEIGHT/grid->unit_rect.h + grid->y_index_first );
}



int main ( int argc, char** argv )
{
	if ( !init_SDL() ) { return -1; }	

	// Create 2d array of cells of size POPULATION x POPULATION
	Cells cells( POPULATION );
	if ( cells.allocated < 0 ) { return -1; }


	//----------------------------------Auxiliary Variables-----------------------------------//

	// To keep track of pan/zoom setting.
	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/POPULATION) , (WINDOW_HEIGHT/POPULATION) };
	Gridmap grid = { 0, POPULATION-1, 0, POPULATION-1, unit_rect };

	INIT_TYPE init_type = INIT_TYPE::IMAGE;

	SDL_Event event;
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
			quit = cells.init_by_user( &grid );
		break;

		case INIT_TYPE::IMAGE:
			quit = cells.init_by_imag( IMAGE_PATH, IMAGE_PPC, ACTIVE_COLOR );
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
		cells.render( &grid );
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

			handle_pan_zoom( &grid, event );
		}		

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );
		
		cells.update();
		cells.render( &grid );

		if ( GRID_SHOWN ) { SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF ); }


		SDL_RenderPresent( renderer );
		SDL_Delay(100);
	}


	quit_SDL();
	return 0;
}