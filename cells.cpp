#include "cells.h"

Cells::Cells( int population ) : POPULATION(population), FRAMED_SIZE(population+2)
{
	//TODO: Add proper memory allocation failure handling.

	// Allocate main array
	cells = (unsigned char**)malloc( FRAMED_SIZE*sizeof(*cells) );
	if ( cells == NULL )
	{
		fprintf( stderr, "ERROR On function Cells::Cells Failed to allocate memory on cells\n" );
		check_error = -1;
	}
	else
	{
		// Allocate columns
		for ( int i = 0; i < FRAMED_SIZE; i++ )
		{
			cells[i] = (unsigned char*)malloc( FRAMED_SIZE*sizeof(**cells) );
			if ( cells[i] == NULL )
			{
				fprintf( stderr, "ERROR On function Cells::Cells Failed to allocate memory on columns\n" );
				check_error = -2;
			}
		}

		if ( check_error >= 0 )
		{
			// Initialise all cells to be 0, isn't necessary, just a redundant error checking
			for ( int i = 0; i < FRAMED_SIZE; i++ )
			{
				for ( int j = 0; j < FRAMED_SIZE; j++ )
				{
					cells[i][j] = 0;
				}
			}

			// Allocate buffers
			prev_col = (unsigned char*)malloc( sizeof( FRAMED_SIZE * (**cells) ) );
			curr_col = (unsigned char*)malloc( sizeof( FRAMED_SIZE * (**cells) ) );
			next_col = (unsigned char*)malloc( sizeof( FRAMED_SIZE * (**cells) ) );
			
			if ( prev_col == NULL ||curr_col == NULL || next_col == NULL )
			{
				fprintf( stderr, "ERROR On function Cells::Cells Failed to allocate memory on buffers\n" );
				check_error = -3;
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




int Cells::update()
{
	//TODO: Optimise for partial updating, don't iterate the regions that are guaranteed to remain unchanged for the next clock

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

		if ( i+2 >= POPULATION+2 ) //Should be a way around this...
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


int Cells::render( Gridmap grid, SDL_Rect unit_rect, int sub_window_width, int sub_window_heigh )
{
	if ( grid.x_start_index < 0 || grid.x_end_index >= POPULATION || grid.y_start_index < 0 || grid.y_end_index >= POPULATION )
	{
		fprintf( stderr, "ERROR On function Cells::render Invalid indecies.\n" );
		return -1;
	}
	
	SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );

	for ( int i = grid.x_start_index; i <= grid.x_end_index; i++ )
	{
		for ( int j = grid.y_start_index; j <= grid.y_end_index; j++ )
		{
			unit_rect.x = i * unit_rect.w;
			unit_rect.y = j * unit_rect.h;

			if ( cells[i+1][j+1] == 1 )
			{
				SDL_RenderFillRect( renderer, &unit_rect );
			}		
		}
	}

	return 0;
}




int Cells::init_by_user( Gridmap grid, SDL_Rect unit_rect )
{
	if ( grid.x_start_index < 0 || grid.x_end_index >= POPULATION || grid.y_start_index < 0 || grid.y_end_index >= POPULATION )
	{
		fprintf( stderr, "ERROR On function Cells::init_by_user Invalid indecies.\n" );
		return -1;
	}

	int mouse_x = 0;
	int mouse_y = 0;
	bool done = false;
	SDL_Event event;

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
					cells[ 1 + grid.x_start_index + i ][ 1 + grid.y_start_index + j ] = 1;

					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xff, 0xff );
					SDL_RenderFillRect( renderer, &unit_rect );
				}
				else if ( event.button.button == SDL_BUTTON_RIGHT ) // Set dead
				{
					cells[ 1 + grid.x_start_index + i ][ 1 + grid.y_start_index + j ] = 0;
			
					SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
					SDL_RenderFillRect( renderer, &unit_rect );
				}

				SDL_RenderPresent( renderer );
			}
		}
	}

	return 0;
}


int Cells::init_by_imag( const char* path )
{
	SDL_Surface* image_surface = SDL_LoadBMP( path );
	if ( image_surface == NULL )
	{
		fprintf( stderr, "ERROR: On function Cells::init_by_imag Failed to load image: %s\n", SDL_GetError() );
		return -1;
	}

	int bpp = image_surface->format->BytesPerPixel;
	
	// TODO: Add a proper error checking here.

	// Surface width and height are in bytes, not bits!

	for ( int j = 0; j < image_surface->h; j++ )
	{
		for ( int i = 0; i < image_surface->w; i++ )
		{
			Uint8* index = (Uint8*)image_surface->pixels + j*image_surface->pitch + i*bpp;
			cells[i+1][j+1] = *index;
		}
	}

	SDL_FreeSurface( image_surface );
	image_surface = NULL;

	return 0;
}


int Cells::init_by_rand()
{
	for ( int i = 1; i < POPULATION+1; i++ )
	{
		for ( int j = 1; j < POPULATION+1; j++ )
		{
			cells[i][j] = rand()%2;
		}
	}

	return 0;
}





