#include"environment.h"

SDL_Window*	  window   = NULL;
SDL_Renderer* renderer = NULL;


bool init_SDL( Env env )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Failed to initialise SDL: %s\n", SDL_GetError() );
		return false;
	}

	if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		fprintf( stderr, "WARNING: I don't know why this matters.\n" );
	}

	if ( !SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software") )
	{
		fprintf( stderr, "Software rendering not enabled.\n" );
	}

	window = SDL_CreateWindow( "Game of Life", 
								SDL_WINDOWPOS_CENTERED, 
								SDL_WINDOWPOS_CENTERED,
								env.window_w, env.window_h,
								SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if ( window == nullptr )
	{
		fprintf( stderr, "Failed to initialise window: %s\n", SDL_GetError() );
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( renderer == nullptr )
	{
		fprintf( stderr, "Failed to create renderer: %s\n", SDL_GetError() );
		return false;
	}

	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );

	return true;
}


void quit_SDL()
{
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	window	 = NULL;
	renderer = NULL;

	SDL_Quit();
}


// TODO: Needs to work with Gridmap
//void SDL_DrawSquareGrid( int cell_count, Uint32 color, int start_x, int end_x, int start_y, int end_y )
//{
//	double x_dist = (end_x - start_x) / (double)cell_count;
//	double y_dist = (end_y - start_y) / (double)cell_count;
//
//	if ( x_dist <= 0 || y_dist <= 0 ) // Should learn Cpp exceptions sometime.
//	{
//		fprintf( stderr, "ERROR: On function SDL_DrawSquareGrid: Invalid parameters\n" );
//		return;
//	}
//
//	Uint8 a = color & 0xFF;
//
//	color >>= 8;
//	Uint8 b = color & 0xFF;
//
//	color >>= 8;
//	Uint8 g = color & 0xFF;
//
//	color >>= 8;
//	Uint8 r = color & 0xFF;
//
//	SDL_SetRenderDrawColor( renderer, r, g, b, a );
//
//
//
//	for ( double i = start_x; i <= end_x; i += x_dist )
//	{
//		SDL_RenderDrawLine( renderer, i, start_y, i, end_y );
//	}
//
//	for ( double i = start_y; i <= end_y; i += y_dist )
//	{
//		SDL_RenderDrawLine( renderer, start_x, i, end_x, i );
//	}
//
//	SDL_RenderDrawLine( renderer, 0, end_y - 1, WINDOW_WIDTH, end_y - 1 );
//	SDL_RenderDrawLine( renderer, end_x - 1, 0, end_x - 1, WINDOW_HEIGHT );
//
//}



void get_options( int argc, char** argv, Env* env )
{
	bool img_defined = false;
	int  img_param = 0; // For if user defines image related options but chose another init type.

	if ( argc < 2 )
	{
		printf( "No arguments are passed. Default values are as follows. Type ? for help, Y to confirm, N to terminate program.\n>>" );

		char input = getchar();
		
		switch ( input )
		{
			case 'Y': case 'y':
				printf( "Confirmed. Continuing program...\n" );
				return;
			break;

			case 'N': case 'n':
				printf( "Quitting...\n" );
				exit(EXIT_SUCCESS);
			break;

			case '?':
				printf( "DONT PANIC!\n" );
				exit(EXIT_SUCCESS);
			break;

			default:
				printf( "What?\n" );
				exit(EXIT_FAILURE);
			break;
		}		
	}


	for ( int i = 1; i < argc; i++ )
	{
		//-----------------------------------------Population-----------------------------------------------//
		if ( strncmp( argv[i], "-p", 2 ) == 0 )
		{
			env->population = strtoul( argv[i+1], NULL, 10 );
			printf( "Population is set to %d\n", env->population );
			i++;
		}
		//-----------------------------------------Image Path-----------------------------------------------//
		else if ( strncmp( argv[i], "--path", 6 ) == 0 )
		{
			img_defined = true;
			img_param++;
			env->init_type = INIT_TYPE::IMAGE;

			strncpy( env->image_path, argv[i+1], 999 );
			FILE* image = fopen( env->image_path, "r" );
			if ( image != NULL )
			{
				fclose( image );
				printf( "Image path is set to <%s>\n", env->image_path );
			}
			else
			{
				fprintf( stderr, "Failed to locate file %s\n", env->image_path );
				exit(EXIT_FAILURE);
			}

			i++;
		}
		//-----------------------------------------Image ppc------------------------------------------------//
		else if ( strncmp( argv[i], "-ppc", 4 ) == 0 )
		{
			img_defined = true;
			img_param++;
			env->image_ppc = strtoul( argv[i+1], NULL, 10 );
			i++;
		}
		//------------------------------------------Image Color---------------------------------------------//
		else if ( strncmp( argv[i], "--color", 7 ) == 0 )
		{
			img_defined = true;
			img_param++;
			if ( strncmp( argv[i+1], "white", 5 ) == 0 )
			{
				env->image_alive_color = 1;
			}
			else if ( strncmp( argv[i+1], "black", 5 ) == 0 )
			{
				env->image_alive_color = 0;
			}
			else
			{
				fprintf( stderr, "--color flag takes one of \"white\", \"black\" options, %s isn't recognised.\n" );
				exit(EXIT_FAILURE);
			}

			i++;
		}
		//-------------------------------------------Init Type-----------------------------------------------//
		else if ( strncmp( argv[i], "-i", 2 ) == 0 )
		{
			if ( strncmp( argv[i+1], "man", 3 ) == 0 && !img_defined ) 
			{
				env->init_type = INIT_TYPE::USER; printf("Selected manual.\n"); 
			}
			else if( strncmp( argv[i+1], "ran", 3 ) == 0 && !img_defined ) 
			{ 
				env->init_type = INIT_TYPE::RANDOM; printf("Selected random.\n"); 
			}
			else if( strncmp( argv[i+1], "im" , 2 ) == 0 ) 
			{ 
				env->init_type = INIT_TYPE::IMAGE; printf("Selected image.\n"); 
			}
			else if ( img_defined )
			{
				fprintf( stderr, "Conflicting options detected.\n" );
				exit(EXIT_FAILURE);
			}
			else
			{
				fprintf( stderr, "-i flag takes one of \"manual\", \"random\", \"image\" arguments. %s isn't recognised.\n", argv[i+1] );
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if ( strncmp( argv[i], "--grid", 6 ) == 0 )
		{
			if ( strncmp( argv[i+1], "shown", 5 ) == 0 )
			{
				env->grid_shown = true;
			}
			else if ( strncmp( argv[i+1], "hidden", 6 ) == 0 )
			{
				env->grid_shown = false;
			}
			else
			{
				fprintf( stderr, "--grid flag takes one of \"shown\", \"hidden\" arguments. %s isn't recognised.\n", argv[i+1] );
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else
		{
			fprintf( stderr, "%s isn't recognised as a valid command or argument.\n", argv[i] );
			exit(EXIT_FAILURE);
		}
	}
	//-------------------------------------------------------------------------------------------------------//

	// Image initialisation requires 3 parameters.
	if ( img_defined && img_param < 3 )
	{
		fprintf( stderr, "Not enough arguments provided for initialising with an image.\n" );
		exit(EXIT_FAILURE);
	}
}


void handle_pan_zoom( Env* env, Map* map, SDL_Event& event )
{
	bool		  update = false;
	static bool	  held   = false;
	static int	  prev_x = 0;
	static int	  prev_y = 0;
	int			  curr_x = 0;
	int			  curr_y = 0;

	// Window resize handling
	if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
	{
		env->window_w = event.window.data1;
		env->window_h = event.window.data2;

		update = true;
	}
	// Zoom handling
	else if ( event.type == SDL_MOUSEWHEEL ) 
	{
		map->unit_rect.w += event.wheel.y * env->zoom_factor;
		map->unit_rect.h  = map->unit_rect.w;

		update = true;
	}
	// Pan handling
	else if ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == BUTTON_PAN )
	{
		held = true;
		SDL_GetMouseState( &prev_x, &prev_y );
	}
	else if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == BUTTON_PAN )
	{
		held = false;

		prev_x = 0;
		prev_y = 0;
	}

	if ( held )
	{
		SDL_GetMouseState( &curr_x, &curr_y );

		map->unit_rect.x -= curr_x - prev_x;
		map->unit_rect.y -= curr_y - prev_y;

		prev_x = curr_x;
		prev_y = curr_y;

		update = true;
	}

	if ( update )
	{
		// Clamp pan tracker

		if ( map->unit_rect.x < 0 ) { map->unit_rect.x = 0; }
		if ( map->unit_rect.y < 0 ) { map->unit_rect.y = 0; }

		// Update indecies

		map->x_index_first = map->unit_rect.x/map->unit_rect.w;
		map->y_index_first = map->unit_rect.y/map->unit_rect.h;

		map->x_index_last  = map->x_index_first + ceil( (double)env->window_w / map->unit_rect.w );
		map->y_index_last  = map->y_index_first + ceil( (double)env->window_h / map->unit_rect.h );

		// Clamp indecies

		if ( map->x_index_first < 0 )				{ map->x_index_first = 0; }	
		if ( map->x_index_last  > env->population )	{ map->x_index_last  = env->population; }
		if ( map->y_index_first < 0 )				{ map->y_index_first = 0; }
		if ( map->y_index_last  > env->population )	{ map->y_index_last  = env->population; }

		// Update border rectangle

		map->border_rect.w = map->unit_rect.w * ( map->x_index_last - map->x_index_first );
		map->border_rect.h = map->unit_rect.h * ( map->y_index_last - map->y_index_first );

	}
}


// Will generalise this
void SDL_DrawGrid( Env env, Map map )
{
	Uint32 color = env.grid_color;
	Uint8 a = color & 0xFF;

	color >>= 8;
	Uint8 b = color & 0xFF;

	color >>= 8;
	Uint8 g = color & 0xFF;

	color >>= 8;
	Uint8 r = color & 0xFF;

	SDL_SetRenderDrawColor( renderer, r, g, b, a );

	// Vertical lines
	for ( int i = 0; i <= env.window_w ; i += map.unit_rect.w )
	{
		SDL_RenderDrawLine( renderer, i, 0, i, env.window_h );
	}
	
	// Horizontal lines
	for ( int j = 0; j <= env.window_h; j += map.unit_rect.h )
	{
		SDL_RenderDrawLine( renderer, 0, j, env.window_w, j );
	}
}




