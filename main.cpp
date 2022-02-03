#include "main.h"
#include<Windows.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

const int POPULATION = 20;

bool flag = false;




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

void print2console( unsigned char** cells, int size_x, int size_y )
{
	for ( int j = 1; j < size_y; j++ )
	{
		for ( int i = 1; i < size_x; i++ )
		{
			printf( "%d ", cells[i][j] );

			int neighbours = cells[i+1][j] + cells[i+1][j+1] + cells[i][j+1] + cells[i-1][j+1] + cells[i-1][j] + cells[i-1][j-1] + cells[i][j-1] + cells[i+1][j-1];	
			
			if ( neighbours < 2 )
			{
				cells[i][j] = 0;
			}
			else if ( neighbours > 3 )
			{
				cells[i][j] = 0;
			}
			else
			{
				cells[i][j] = 1;
			}

			

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

	

	unsigned char** cells;

	cells = (unsigned char**)malloc( (POPULATION+2) * sizeof(*cells) );
	for ( int i = 0; i < (POPULATION+2); i++ )
	{
		cells[i] = (unsigned char*)malloc( (POPULATION+2) * sizeof cells[0] );
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
	cells[6][5] = 1;
	cells[6][6] = 1;


	
	



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

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderPresent( renderer );

		SDL_DrawSquareGrid( POPULATION, 0xFFFFFFFF );
		SDL_RenderPresent( renderer );

		for ( int i = 0; i <= 1; i++ )
		{
			//printf("\x1B[1;1H\x1B[2J");

			printf("---------------------------------------------------------------------------------------\n");


			//update( cells, POPULATION, POPULATION, unit_rect );
			print2console( cells, POPULATION, POPULATION );

			

			SDL_Delay(1000);

			flag = true;
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