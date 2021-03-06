#include "cells.h"


Cells::Cells( int population ) : POPULATION(population), FRAMED_SIZE(population+2)
{
	allocated = 0;

	// Allocate main array
	cells = (unsigned char**)malloc( FRAMED_SIZE*sizeof(*cells) );

	if ( cells == NULL )
	{
		fprintf( stderr, "Error in %s() : Failed to allocate memory on cells\n", __FUNCTION__ );
		allocated = -1;
	}
	else
	{
		// Allocate columns
		for ( int i = 0; i < FRAMED_SIZE; i++ )
		{
			cells[i] = (unsigned char*)malloc( FRAMED_SIZE*sizeof(**cells) );

			if ( cells[i] == NULL )
			{
				fprintf( stderr, "Error in %s() : Failed to allocate memory on columns\n", __FUNCTION__ );
				allocated = -2;
			}
		}

		if ( allocated >= 0 )
		{
			// Initialise all cells to be 0, isn't necessary, just a redundant error checking
			for ( int j = 0; j < FRAMED_SIZE; j++ )
			{
				for ( int i = 0; i < FRAMED_SIZE; i++ )
				{
					cells[i][j] = 0;
				}
			}

			// Allocate buffers
			prev_col = (unsigned char*)malloc( FRAMED_SIZE * sizeof(**cells) );
			curr_col = (unsigned char*)malloc( FRAMED_SIZE * sizeof(**cells) );
			next_col = (unsigned char*)malloc( FRAMED_SIZE * sizeof(**cells) );
			
			if ( prev_col == NULL ||curr_col == NULL || next_col == NULL )
			{
				fprintf( stderr, "Error in %s() : Failed to allocate memory on buffers\n", __FUNCTION__ );
				allocated = -3;
			}
		}
	}	
}


Cells::~Cells()
{
	for ( int i = 0; i < FRAMED_SIZE; i++ )
	{
		free( cells[i] );
		cells[i] = NULL;
	}
	free( cells );
	cells = NULL;

	free( prev_col );
	prev_col = NULL;

	free( curr_col );
	curr_col = NULL;

	free( next_col );
	next_col = NULL;
}


/*
* What if, instead of holding each cell as a byte, hold the surrounding 8 cells in one byte instead?
*				0 1 2
*				3 i 4	==> i = 0b(01234567)
*				5 6 7
* 
* sure it would have redundancy over the neighbouring edges, but still would occupy less space.
* It also would allow use of direct bitwise operations in determining whether a cell is alive or dead.
* 
* Do a benchark on this...
*	
*/

int Cells::update()
{	//TODO: Optimise for partial updating, don't iterate the regions that are guaranteed to remain unchanged for the next clock

	// Copy initial position to buffers
	memcpy( prev_col, cells[0], FRAMED_SIZE * sizeof(**cells) );
	memcpy( curr_col, cells[1], FRAMED_SIZE * sizeof(**cells) );
	memcpy( next_col, cells[2], FRAMED_SIZE * sizeof(**cells) );

	for ( int i = 1; i < POPULATION+1; i++ )
	{
		for ( int j = 1; j < POPULATION+1; j++ )
		{
			// Read from buffers...
			int neighbours	= prev_col[j-1] + curr_col[j-1] + next_col[j-1]
							+ prev_col[j]					+ next_col[j] 						
							+ prev_col[j+1] + curr_col[j+1] + next_col[j+1];

			
			// ...write to the actual array
			if ( neighbours < 2 || neighbours > 3 )
			{
				cells[i][j] = 0;
			}
			else if ( neighbours == 3 )
			{
				cells[i][j] = 1;
			}
		}

		if ( i+2 >= POPULATION+2 ) //Should be a better way around this...
		{
			break;
		}

		// Update buffers for next iteration
		memcpy( prev_col, curr_col,   FRAMED_SIZE );
		memcpy( curr_col, next_col,   FRAMED_SIZE );
		memcpy( next_col, cells[i+2], FRAMED_SIZE );		
	}

	return 0;
}

// Doesn't support cells smaller than 1 pixel, will be completely changed.
int Cells::render( Map map )
{
	SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );

	SDL_Rect unit_rect = map.unit_rect;

	for ( int j = map.y_index_first; j < map.y_index_last; j++ )
	{
		for ( int i = map.x_index_first; i < map.x_index_last; i++ )
		{
			unit_rect.x = i * unit_rect.w + map.unit_rect.x;
			unit_rect.y = j * unit_rect.h + map.unit_rect.y;

			if ( cells[i+1][j+1] == 1 )
			{
				SDL_RenderFillRect( renderer, &unit_rect );
			}		
		}
	}

	return 0;
}


int Cells::init_by_user( Env env, Map map )
{//TODO: This probably shouldn't have its own event loop.

	int mouse_x = 0;
	int mouse_y = 0;
	bool done = false;
	SDL_Event event;
	SDL_Rect unit_rect = map.unit_rect;

	while ( !done )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				return -2;
			}
			else if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN ) // If done...
			{
				done = true;
			}
			else if ( event.type == SDL_MOUSEBUTTONDOWN )
			{
				SDL_GetMouseState( &mouse_x, &mouse_y );

				// Calculate corresponding **relative** indecies, absolute index is 1 + grid.<x/y>_start_index + <i/j>
				int i = mouse_x / unit_rect.w;
				int j = mouse_y / unit_rect.h;

				// Get coordinates of the corresponding square
				unit_rect.x = i * unit_rect.w;
				unit_rect.y = j * unit_rect.h;

				//TODO: ADD SINGLE CAPTURE IT REPEATS THE SHIT OUT OF THIS
				if ( event.button.button == SDL_BUTTON_LEFT ) // Set alive
				{
					cells[ 1 + map.x_index_first + i ][ 1 + map.y_index_first + j ] = 1;

					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xff, 0xff );
					SDL_RenderFillRect( renderer, &unit_rect );
				}
				else if ( event.button.button == SDL_BUTTON_RIGHT ) // Set dead
				{
					cells[ 1 + map.x_index_first + i ][ 1 + map.y_index_first + j ] = 0;
			
					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
					SDL_RenderFillRect( renderer, &unit_rect );
				}				
			}
		}

		SDL_DrawGrid( env, map );
		SDL_RenderPresent( renderer );
	}

	return 0;
}


int Cells::init_by_imag( const char* path, int cell_length, int active_color )
{
	int offset = cell_length / 2;

	SDL_Surface* image_surface = SDL_LoadBMP( path );
	if ( image_surface == NULL )
	{
		fprintf( stderr, "Error in %s() : Failed to load image: %s\n", __FUNCTION__, SDL_GetError() );
		return -1;
	}
	else if ( image_surface->format->BytesPerPixel != 1 )
	{
		fprintf( stderr, "Error in %s() : Only accepting monochrome bmp images.\n", __FUNCTION__ );
		return -1;
	}
	else if ( image_surface->h / cell_length >= POPULATION  ||  image_surface->w / cell_length >= POPULATION )
	{
		fprintf( stderr, "Warning in %s() : <%s> contains too much information and will be truncated.\n", __FUNCTION__, path );
	}

	// Iterate through selected pixels and write to the cells. Exits if the end of image or cells array is encountered.
	for ( int j = 0; j < POPULATION; j++ )
	{
		int grab_y = offset + j*cell_length;
		if ( grab_y >= image_surface->h ) { break; }


		for ( int i = 0; i < POPULATION; i++ )
		{
			int grab_x = offset + i*cell_length;
			if ( grab_x >= image_surface->w ) { break; }

			Uint8* index = (Uint8*)image_surface->pixels + grab_y*image_surface->pitch + grab_x;
			cells[i+1][j+1] = ( *index == active_color ); 
		}
	}

	// Turns out you don't free( an SDL surface pointer ) yourself ... 
	SDL_FreeSurface( image_surface );
	image_surface = NULL;

	return 0;
}


int Cells::init_by_rand()
{
	for ( int j = 1; j < POPULATION+1; j++ )
	{
		for ( int i = 1; i < POPULATION+1; i++ )
		{
			cells[i][j] = rand()%2;
		}
	}

	return 0;
}





