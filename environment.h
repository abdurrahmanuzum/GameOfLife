#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define BUTTON_PAN SDL_BUTTON_LEFT

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>

extern SDL_Window*	 window;
extern SDL_Renderer* renderer;

enum class INIT_TYPE { USER, IMAGE, FILE, RANDOM };

typedef struct
{
	int x_index_first;
	int x_index_last;
	int y_index_first;
	int y_index_last;
	SDL_Rect unit_rect;
	SDL_Rect border_rect; 

}Map;

typedef struct
{
	int		window_w;
	int		window_h;
	int		zoom_factor;
	bool	grid_shown;
	Uint32	grid_color;

	int  population;

	char image_path[1000];
	int  image_ppc;
	int  image_alive_color;

	INIT_TYPE init_type;

}Env;

bool init_SDL( Env env );
void quit_SDL();
void get_options( int argc, char** argv, Env* env );
void handle_pan_zoom( Env* env, Map* map, SDL_Event& event );

void SDL_DrawGrid( Env env, Map map );






