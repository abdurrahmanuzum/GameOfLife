#include"environment.h"

SDL_Window*	  window   = NULL;
SDL_Renderer* renderer = NULL;


bool init_SDL( Env env )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Error in %s() : Failed to initialise SDL: %s\n", __FUNCTION__,  SDL_GetError() );
		return false;
	}

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

	
	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
	

	window = SDL_CreateWindow( "Game of Life", 
								SDL_WINDOWPOS_CENTERED, 
								SDL_WINDOWPOS_CENTERED,
								env.window_w, env.window_h,
								SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if ( window == nullptr )
	{
		fprintf( stderr, "Error in %s() : Failed to create window: %s\n", __FUNCTION__, SDL_GetError() );
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( renderer == nullptr )
	{
		fprintf( stderr, "Error in %s() : Failed to create renderer: %s\n", __FUNCTION__, SDL_GetError() );
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
		if ( strcmp( argv[i], "-p" ) == 0 && strlen(argv[i]) == 2 )
		{
			env->population = strtoul( argv[i+1], NULL, 10 );
			printf( "Population is set to %d\n", env->population );
			i++;
		}
		//-----------------------------------------Image Path-----------------------------------------------//
		else if ( strcmp( argv[i], "--path" ) == 0 && strlen(argv[i]) == 6 )
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
				fprintf( stderr, "Error in %s() : Failed to locate file <%s>\n", __FUNCTION__, env->image_path );
				exit(EXIT_FAILURE);
			}

			i++;
		}
		//-----------------------------------------Image ppc------------------------------------------------//
		else if ( strcmp( argv[i], "-ppc" ) == 0 && strlen(argv[i]) == 4 )
		{
			img_defined = true;
			img_param++;
			env->image_ppc = strtoul( argv[i+1], NULL, 10 );
			i++;
		}
		//------------------------------------------Image Color---------------------------------------------//
		else if ( strcmp( argv[i], "--color" ) == 0 && strlen(argv[i]) == 7 )
		{
			img_defined = true;
			img_param++;
			if ( strcmp( argv[i+1], "white" ) == 0 )
			{
				env->image_alive_color = 1;
			}
			else if ( strcmp( argv[i+1], "black" ) == 0 )
			{
				env->image_alive_color = 0;
			}
			else
			{
				fprintf( stderr, "Error in %s() : --color flag takes one of \"white\", \"black\" options, <%s> isn't recognised.\n", __FUNCTION__, argv[i+1] );
				exit(EXIT_FAILURE);
			}

			i++;
		}
		//-------------------------------------------Init Type-----------------------------------------------//
		else if ( strcmp( argv[i], "-i" ) == 0 && strlen(argv[i]) == 2 )
		{
			if ( strncmp( argv[i+1], "man", 3 ) == 0 && !img_defined ) 
			{
				env->init_type = INIT_TYPE::USER; printf("Selected manual.\n"); 
			}
			else if( strncmp( argv[i+1], "ran", 3 ) == 0 && !img_defined ) 
			{ 
				env->init_type = INIT_TYPE::RANDOM; printf("Selected random.\n"); 
			}
			else if( strncmp( argv[i+1], "im", 2 ) == 0 ) 
			{ 
				img_defined = true;
				env->init_type = INIT_TYPE::IMAGE; printf("Selected image.\n"); 
			}
			else if ( img_defined )
			{
				fprintf( stderr, "Error in %s() : Conflicting options detected.\n", __FUNCTION__ );
				exit(EXIT_FAILURE);
			}
			else
			{
				fprintf( stderr, "Error in %s() : -i flag takes one of \"manual\", \"random\", \"image\" arguments. %s isn't recognised.\n", __FUNCTION__, argv[i+1] );
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if ( strcmp( argv[i], "--grid" ) == 0 && strlen(argv[i]) == 6 )
		{
			if ( strcmp( argv[i+1], "shown" ) == 0 )
			{
				env->grid_shown = true;
			}
			else if ( strcmp( argv[i+1], "hidden" ) == 0 )
			{
				env->grid_shown = false;
			}
			else
			{
				fprintf( stderr, "Error in %s() : --grid flag takes one of \"shown\", \"hidden\" arguments. %s isn't recognised.\n", __FUNCTION__, argv[i+1] );
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else
		{
			fprintf( stderr, "Error in %s() : <%s> isn't recognised as a valid command or argument.\n", __FUNCTION__, argv[i] );
			exit(EXIT_FAILURE);
		}
	}
	//-------------------------------------------------------------------------------------------------------//

	// Image initialisation requires 3 parameters.
	if ( img_defined && img_param < 3 )
	{
		fprintf( stderr, "Error in %s() : Not enough arguments provided for initialising with an image.\n", __FUNCTION__ );
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
		map->unit_rect.w *= pow( 2, event.wheel.y );
		map->unit_rect.h  = map->unit_rect.w;
		
		SDL_GetMouseState( &curr_x, &curr_y );

		if ( event.wheel.y > 0 )
		{
			map->unit_rect.x = 2*map->unit_rect.x - curr_x;
			map->unit_rect.y = 2*map->unit_rect.y - curr_y;
		}
		else
		{
			map->unit_rect.x = curr_x - 0.5*(curr_x - map->unit_rect.x);
			map->unit_rect.y = curr_y - 0.5*(curr_y - map->unit_rect.y);
		}

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

		map->unit_rect.x += ( curr_x - prev_x );
		map->unit_rect.y += ( curr_y - prev_y );

		prev_x = curr_x;
		prev_y = curr_y;

		update = true;
	}

	if ( update )
	{
		// Clamp pan tracker

		if ( map->unit_rect.w < 1 ) { map->unit_rect.w = 1; }
		if ( map->unit_rect.h < 1 ) { map->unit_rect.h = 1; }

		// Update indecies

		map->x_index_first = floor( -1.0 * map->unit_rect.x / map->unit_rect.w );
		map->y_index_first = floor( -1.0 * map->unit_rect.y / map->unit_rect.h );

		map->x_index_last  = map->x_index_first + ceil( (double)env->window_w / map->unit_rect.w );
		map->y_index_last  = map->y_index_first + ceil( (double)env->window_h / map->unit_rect.h );

		// Clamp indecies

		if ( map->x_index_first < 0 )				{ map->x_index_first = 0; }	
		if ( map->x_index_last  > env->population )	{ map->x_index_last  = env->population; }
		if ( map->y_index_first < 0 )				{ map->y_index_first = 0; }
		if ( map->y_index_last  > env->population )	{ map->y_index_last  = env->population; }

		// Update border rectangle

		map->border_rect.x = map->unit_rect.x;
		map->border_rect.y = map->unit_rect.y;
		map->border_rect.w = map->unit_rect.w * map->x_index_last;
		map->border_rect.h = map->unit_rect.h * map->y_index_last;
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




