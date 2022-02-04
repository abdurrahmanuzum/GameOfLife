#include "main.h"
#include<Windows.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

const int POPULATION = 20;



void update( unsigned char** cells, int size_x, int size_y, SDL_Rect unit_rect )
{
	for ( int j = 1; j < size_y; j++ )
	{
		for ( int i = 1; i < size_x; i++ )
		{
			unit_rect.x = (i-1)*unit_rect.w;
			unit_rect.y = (j-1)*unit_rect.h;


			int neighbours = cells[i+1][j] + cells[i+1][j+1] + cells[i][j+1] + cells[i-1][j+1] + cells[i-1][j] + cells[i-1][j-1] + cells[i][j-1] + cells[i+1][j-1];	
			
			if ( neighbours < 2 )
			{
				cells[i][j] = 0;
				SDL_SetRenderDrawColor( renderer, 0x00, 0x30, 0x00, 0xff );
			}
			else if ( neighbours > 3 )
			{
				cells[i][j] = 0;
				SDL_SetRenderDrawColor( renderer, 0x00, 0x30, 0x00, 0xff );
			}
			else
			{
				cells[i][j] = 1;
				SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );
			}

			SDL_RenderFillRect( renderer, &unit_rect );
		}
	}
}

void update2( unsigned char** cells, int size )
{
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

		if ( i+2 < size+2 )
		{
			memcpy( prev_col, curr_col, size+2 );
			memcpy( curr_col, next_col, size+2 );
			memcpy( next_col, cells[i+2], size+2 );
		}
	}
}

void print2console( unsigned char** cells, int size_x, int size_y )
{
	for ( int j = 0; j < size_y; j++ )
	{
		for ( int i = 0; i < size_x; i++ )
		{
			printf( "%d ", cells[i][j] );

			//int neighbours = cells[i+1][j] + cells[i+1][j+1] + cells[i][j+1] + cells[i-1][j+1] + cells[i-1][j] + cells[i-1][j-1] + cells[i][j-1] + cells[i+1][j-1];	
			
			
		}
		printf("\n");
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
	

	printf( "Initial condition map is as follows:\n\n" );
	print2console( cells, POPULATION+2, POPULATION+2 );
	system("pause");


	bool quit = false;
	SDL_Event event;

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		//SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		//SDL_RenderPresent( renderer );

		//SDL_DrawSquareGrid( POPULATION, 0xFFFFFFFF );
		//SDL_RenderPresent( renderer );

		for ( int i = 0; i <= 3; i++ )
		{
			//printf("\x1B[1;1H\x1B[2J");

			printf("---------------------------------------------------------------------------------------\n");


			//update( cells, POPULATION, POPULATION, unit_rect );
			update2( cells, POPULATION );
			print2console( cells, POPULATION+2, POPULATION+2 );

			

			SDL_Delay(1000);
		}
		break;
	}


	for ( int i = 0; i < (POPULATION+2); i++ )
	{
		free( cells[i] );
	}
	free( cells );


	quit_SDL();
	return 0;
}