#pragma once

#include "main.h"

class Cells
{
	private:
		const int POPULATION;
		const int FRAMED_SIZE;

		unsigned char** cells;

		unsigned char* prev_col;
		unsigned char* curr_col;
		unsigned char* next_col;

		

	public:
		Cells( int population );
		~Cells();
		int update();
		int render( Gridmap grid, SDL_Rect unit_rect, int sub_window_width = WINDOW_WIDTH, int sub_window_height = WINDOW_HEIGHT );
		int init( Gridmap grid, SDL_Rect unit_rect, SDL_Event& event );
		int init_random();
		int init_from_image( const char* path );


};