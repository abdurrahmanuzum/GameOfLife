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
		int allocated;

		

	public:
		Cells( int population );
		~Cells();
		
		int update();
		int render( Gridmap grid, SDL_Rect unit_rect, int sub_window_width = WINDOW_WIDTH, int sub_window_height = WINDOW_HEIGHT );
		
		int init_by_user( Gridmap grid, SDL_Rect unit_rect );
		int init_by_imag( const char* path, int cell_length, int active_color );
		int init_by_rand();

};