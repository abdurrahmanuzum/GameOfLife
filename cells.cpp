#include "cells.h"

Cells::Cells( int population ) : POPULATION(population), FRAMED_SIZE(population+2)
{
	//TODO: Add proper memory allocation failure handling.

	// Allocate main array
	cells = (unsigned char**)malloc( FRAMED_SIZE*sizeof(*cells) );
	if ( cells == NULL )
	{
		check_error = -1;
	}
	else
	{
		for ( int i = 0; i < FRAMED_SIZE; i++ )
		{
			cells[i] = (unsigned char*)malloc( FRAMED_SIZE*sizeof(**cells) );
			if ( cells[i] == NULL )
			{
				check_error = -2;
			}
		}

		if ( check_error >= 0 )
		{
			// Initialise all cells to be 0, isn't necessary, just a redundant error checking mechanism
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
		}
	}	
}


Cells::~Cells()
{
	for ( int i = 0; i < FRAMED_SIZE; i++ )
	{
		free( cells[i] );
	}
	free( cells );

	free( prev_col );
	free( curr_col );
	free( next_col );
}


int Cells::init( Gridmap grid, SDL_Rect unit_rect, SDL_Event& event )
{
	int mouse_x = 0;
	int mouse_y = 0;

	
	if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN ) // If done...
	{
		return 1;
	}

	else if ( event.type == SDL_MOUSEBUTTONDOWN )
	{
		SDL_GetMouseState( &mouse_x, &mouse_y );

		// Calculate corresponding **relative** indecies, absolute index is 1 + grid.<x/y>_start_index + <i/j>
		int i = mouse_x / unit_rect.w;
		int j = mouse_y / unit_rect.h;

		// Getting coordinates of the corresponding square
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
	}

	return 0;
}


int Cells::init_random()
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
		printf( "On function print_to_window:\n" );
		printf( "Invalid indecies detected, aborting printing...\n" );
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