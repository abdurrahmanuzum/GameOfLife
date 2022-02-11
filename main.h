#pragma once

#define _CRT_SECURE_NO_WARNINGS //Fuck off Visual Studio

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern const int WINDOW_WIDTH_DEFAULT;
extern const int WINDOW_HEIGHT_DEFAULT;


bool init_SDL();
void quit_SDL();

//void SDL_DrawSquareGrid( int cell_count, Uint32 color = 0xFFFFFFFF, int start_x = 0, int end_x = WINDOW_WIDTH, int start_y = 0, int end_y );

typedef struct
{
	int x_index_first;
	int x_index_last;
	int y_index_first;
	int y_index_last;
	SDL_Rect unit_rect;
	SDL_Rect border_rect; 

}Gridmap;

enum class INIT_TYPE { USER, IMAGE, FILE, RANDOM };
