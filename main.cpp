#include "main.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;




int main ( int argc, char** argv )
{
	if ( !init_SDL() )
	{
		return -1;
	}


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

		SDL_DrawSquareGrid( 20, 0xFFFFFFFF );
		SDL_RenderPresent( renderer );
	}




	quit_SDL();
	return 0;
}