#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <SDL.h>
#include<SDL_image.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const int POPULATION;


enum class Color { WHITE, BLACK, RED, GREEN, BLUE };

bool init_SDL();
void quit_SDL();

void SDL_DrawSquareGrid( int cell_count, Uint32 color = 0xFFFFFFFF, int start_x = 0, int end_x = WINDOW_WIDTH, int start_y = 0, int end_y = WINDOW_HEIGHT );