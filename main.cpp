#include "main.h"
#include<Windows.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

const int POPULATION = 10;


void update_cells( unsigned char** cells, int size )
{
	//TODO: Optimise for partial updating, don't iterate the regions that are guaranteed to remain unchanged for the next clock

	unsigned char* prev_col = nullptr;
	unsigned char* curr_col = nullptr;
	unsigned char* next_col = nullptr;
	
	prev_col = (unsigned char*)malloc( sizeof(cells[0]) );
	curr_col = (unsigned char*)malloc( sizeof(cells[0]) );
	next_col = (unsigned char*)malloc( sizeof(cells[0]) );

	memcpy( prev_col, cells[0], size+2 );
	memcpy( curr_col, cells[1], size+2 );
	memcpy( next_col, cells[2], size+2 );
	
	for ( int i = 1; i < size+1; i++ )
	{
		for ( int j = 1; j < size+1; j++ )
		{
			int neighbours	= prev_col[j-1] + curr_col[j-1] + next_col[j-1]
							+ prev_col[j]					+ next_col[j] 						
							+ prev_col[j+1] + curr_col[j+1] + next_col[j+1];

			if ( neighbours < 2 || neighbours > 3 )
			{
				cells[i][j] = 0;
			}
			else if ( neighbours == 3 )
			{
				cells[i][j] = 1;
			}
		}

		if ( i+2 < size+2 ) //Should be a way around this...
		{
			memcpy( prev_col, curr_col, size+2 );
			memcpy( curr_col, next_col, size+2 );
			memcpy( next_col, cells[i+2], size+2 );
		}
	}
}


void print_to_console( unsigned char** cells, int size )
{
	for ( int j = 0; j < size; j++ )
	{
		for ( int i = 0; i < size; i++ )
		{
			printf( "%d ", cells[i][j] );
			
		}
		printf("\n");
	}
}

int print_to_window( unsigned char** cells, int size, int x_start_index, int x_end_index, int y_start_index, int y_end_index, SDL_Rect unit_rect, int sub_window_width = WINDOW_WIDTH, int sub_window_height = WINDOW_HEIGHT )
{
	if ( x_start_index < 0 || x_end_index > size || y_start_index < 0 || y_end_index > size )
	{
		printf( "On function print_to_window:\n" );
		printf( "Invalid indecies detected, aborting printing...\n" );
		return -1;
	}

	SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );

	for ( int j = y_start_index; j < y_end_index; j++ )
	{
		for ( int i = x_start_index; i < x_end_index+1; i++ )
		{
			unit_rect.x = i * unit_rect.w;
			unit_rect.y = j * unit_rect.h;

			if ( cells[i+1][j+1] == 1 )
			{
				SDL_RenderFillRect( renderer, &unit_rect );
			}
			else
			{
				SDL_RenderDrawRect( renderer, &unit_rect );
			}			
		}
	}
}






int main ( int argc, char** argv )
{
	if ( !init_SDL() )
	{
		return -1;
	}
	

	unsigned char** cells = nullptr;

	cells = (unsigned char**)malloc( (POPULATION+2) * sizeof(*cells) );
	for ( int i = 0; i < (POPULATION+2); i++ )
	{
		cells[i] = (unsigned char*)malloc( (POPULATION+2) * sizeof(cells[0]) );
	}

	for ( int j = 0; j < (POPULATION+2); j++ )
	{
		for ( int i = 0; i < (POPULATION+2); i++ )
		{
			cells[i][j] = 0;
		}
	}

	SDL_Rect unit_rect = { 0, 0, WINDOW_WIDTH/POPULATION, WINDOW_HEIGHT/POPULATION };


	cells[5][5] = 1;
	cells[5][6] = 1;
	cells[5][7] = 1;


	bool quit = false;
	SDL_Event event;

	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
	SDL_RenderPresent( renderer );

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );

		update_cells( cells, POPULATION );
		printf("Updated.\n");
		print_to_window( cells, POPULATION, 0, 10, 0, 10, unit_rect );
		printf("Printed.\n");
		SDL_RenderPresent( renderer );


		SDL_Delay(1000);



	}









	for ( int i = 0; i < (POPULATION+2); i++ )
	{
		free( cells[i] );
	}
	free( cells );


	quit_SDL();
	return 0;
}