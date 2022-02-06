#include"main.h"

/*
* \brief Initialises SDL
* \return true on success, false on failure
*/
bool init_SDL()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "Failed to initialise SDL: %s\n", SDL_GetError() );
		return false;
	}

	if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "WARNING: I don't know why this matters.\n" );
	}

	if ( !SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software") )
	{
		printf( "Software rendering not enabled.\n" );
	}

	window = SDL_CreateWindow( "Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
	if ( window == nullptr )
	{
		printf( "Failed to initialise window: %s\n", SDL_GetError() );
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( renderer == nullptr )
	{
		printf( "Failed to create renderer: %s\n", SDL_GetError() );
		return false;
	}

	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );

	return true;
}


void quit_SDL()
{
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	window = nullptr;
	renderer = nullptr;

	SDL_Quit();
}


void SDL_DrawSquareGrid( int cell_count, Uint32 color, int start_x, int end_x, int start_y, int end_y )
{
	double x_dist = (end_x - start_x) / (double)cell_count;
	double y_dist = (end_y - start_y) / (double)cell_count;

	if ( x_dist <= 0 || y_dist <= 0 ) // Should learn Cpp exceptions sometime.
	{
		printf( "ERROR: On function SDL_DrawSquareGrid: Invalid parameters\n" );
		return;
	}

	Uint8 a = color & 0xFF;

	color >>= 8;
	Uint8 b = color & 0xFF;

	color >>= 8;
	Uint8 g = color & 0xFF;

	color >>= 8;
	Uint8 r = color & 0xFF;

	SDL_SetRenderDrawColor( renderer, r, g, b, a );



	for ( double i = start_x; i <= end_x; i += x_dist )
	{
		SDL_RenderDrawLine( renderer, i, start_y, i, end_y );
	}

	for ( double i = start_y; i <= end_y; i += y_dist )
	{
		SDL_RenderDrawLine( renderer, start_x, i, end_x, i );
	}

	SDL_RenderDrawLine( renderer, 0, end_y - 1, WINDOW_WIDTH, end_y - 1 );
	SDL_RenderDrawLine( renderer, end_x - 1, 0, end_x - 1, WINDOW_HEIGHT );

}