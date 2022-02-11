#pragma once

#include "environment.h"

class Cells
{
	private:
		const int POPULATION;
		const int FRAMED_SIZE; // POPULATION*POPULATION array is framed with one constant 0 element 
							   // in each direction to avoid boundary condition checking.	
		unsigned char** cells;

		unsigned char* prev_col;
		unsigned char* curr_col;
		unsigned char* next_col;

	public:
		int allocated; // Constructor success flag. Why don't constructors return...

		

	public:
		Cells( int population );
		~Cells();
		
		int update();
		int render( Gridmap grid );
		
		int init_by_user( Gridmap grid );
		int init_by_imag( const char* path, int cell_length, int active_color );
		int init_by_rand();

};