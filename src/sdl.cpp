#include <iostream>
#include <algorithm>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>

#include "sdl.hpp"
#include "liquidsim.h"
#include "cfluid.h"

#include "main.hpp"

SdlApp::SdlApp(int width, int height):
    screenWidth{width}, screenHeight{height},
    window{nullptr},
    renderer{nullptr}
{
    //init SDL
    int initErr = SDL_Init( SDL_INIT_VIDEO );
    if( initErr < 0 ) { throw SdlError("SDL could not initialize!"); }

    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
        std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

    //Create window
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
    if( window == nullptr ){ throw SdlError("Window could not be created!"); }

    //Create renderer for window
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == nullptr ) { throw SdlError("Renderer could not be created!"); }

    //Initialize renderer color
    SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x55, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) { throw SdlImgError("SDL_image could not initialize!"); }
}

SdlApp::~SdlApp(){
    //Destroy renderer
    SDL_DestroyRenderer( renderer );

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool SdlApp::draw(){
    //Wait half a second
    SDL_Delay( 100 );

    //Clear screen
    SDL_RenderClear( renderer );

    //Update screen
    SDL_RenderPresent( renderer );



    //Wait half a second
    SDL_Delay( 100 );

	//Clear screen
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( renderer );

    //Render red filled quad
    SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
    SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect( renderer, &fillRect );

    //Render green outlined quad
    SDL_Rect outlineRect = { screenWidth / 6, screenHeight / 6, screenWidth * 2 / 3, screenHeight * 2 / 3 };
    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderDrawRect( renderer, &outlineRect );

    //Draw blue horizontal line
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF );
    SDL_RenderDrawLine( renderer, 0, screenHeight / 2, screenWidth, screenHeight / 2 );

    //Draw vertical line of yellow dots
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
    for( int i = 0; i < screenHeight; i += 4 )
    {
        SDL_RenderDrawPoint( renderer, screenWidth / 2, i );
    }

    //Update screen
    SDL_RenderPresent( renderer );

    //Wait two seconds
	SDL_Delay( 200 );

    return true;
}

#define IX(i,j) ((i)+(M)*(j))
void SdlApp::loop(){
    std::cerr << "init" << std::endl;
    static const int M = 480;//std::max(screenHeight, screenWidth);
    static const int N = M-2;
    static const int M_size = M * M;

    int width = M;
    int height = M;
    SDL_RenderClear( renderer );
    SDL_RenderSetViewport( renderer, nullptr );
    SDL_Rect renderQuad = { 0, 0, width, height };

    SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING;

    SDL_Texture* texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
    if(texture == nullptr) { throw SdlError("Unable to create blank texture!"); }

    //Enable blending on texture
    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureColorMod( texture, 0xff, 0xff, 0xff );

    static float u[M_size], v[M_size], u_prev[M_size], v_prev[M_size];
    static float dens[M_size], dens_prev[M_size];
    static float sources[M_size], force_u[M_size], force_v[M_size];

    static const float visc = 1.512041288; // uPa * s + K^-1/2
    //static const float diff = 1.5e-5; //m^2 * s^1
    static const float diff = 1.5e+0; //m^2 * s^1
    //static const float diff = 4e+2; //m^2 * s^1
    float dt = 1.f;

    LiquidSim sim;

    std::cerr << "set start" << std::endl;
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            int pos_x, pos_y;
            /*
            pos_x = 110;
            pos_y = 130;
            sources[IX(i+pos_x,j+pos_y)] = 100.;
            force_u[IX(i+pos_x,j+pos_y)] = 10. / N;
            force_v[IX(i+pos_x,j+pos_y)] = 1. / N;
            *//*
            pos_x = 25;
            pos_y = 25;
            sources[IX(i+pos_x,j+pos_y)] = 40.;
            force_u[IX(i+pos_x,j+pos_y)] = 2. / N;
            force_v[IX(i+pos_x,j+pos_y)] = 4. / N;
            */
            //////////////////////////////////////

            pos_x = 235;
            pos_y = 235;
            sources[IX(i+pos_x,j+pos_y)] = 40.;
            force_u[IX(i+pos_x,j+pos_y)] = 0. / N;
            force_v[IX(i+pos_x,j+pos_y)] = 0. / N;

            pos_x = 400;
            pos_y = 400;
            sources[IX(i+pos_x,j+pos_y)] = 20.;
            force_u[IX(i+pos_x,j+pos_y)] = -1. / N;
            force_v[IX(i+pos_x,j+pos_y)] = -5. / N;

            pos_x = 400;
            pos_y = 80;
            sources[IX(i+pos_x,j+pos_y)] = 20.;
            force_u[IX(i+pos_x,j+pos_y)] = -5. / N;
            force_v[IX(i+pos_x,j+pos_y)] = 1. / N;

            pos_x = 80;
            pos_y = 80;
            sources[IX(i+pos_x,j+pos_y)] = 20.;
            force_u[IX(i+pos_x,j+pos_y)] = 1. / N;
            force_v[IX(i+pos_x,j+pos_y)] = 5. / N;

            pos_x = 80;
            pos_y = 400;
            sources[IX(i+pos_x,j+pos_y)] = 20.;
            force_u[IX(i+pos_x,j+pos_y)] = 5. / N;
            force_v[IX(i+pos_x,j+pos_y)] = -1. / N;


            if(i<5 && j<5){
                sources[IX(i+10,j+230)] = 10.;
            }

            for(int k=0; k<48; k++)
            {
                pos_x = k*10;
                pos_y = 0;
                sources[IX(i+pos_x,j+pos_y)] = k;
                force_u[IX(i+pos_x,j+pos_y)] = 0. / N;
                force_v[IX(i+pos_x,j+pos_y)] = 1. / N;
            }

        }
    }

    sim.add_source(N, u_prev, force_u, 1.);
    sim.add_source(N, v_prev, force_v, 1.);
    sim.add_source(N, dens, sources, 1.);

    std::cout << "looping" << std::endl;
    bool running = true;
    uint64_t holeTime = 0;
    uint32_t iter = 0;
    while( running && iter < 1000)
    {
        iter++;
        std::cout << "\n--- start iter: " << iter << " ---" << std::endl;
        uint32_t t_0 = SDL_GetTicks();
        SDL_Event e;
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            switch(e.type){
            //User requests quit
            case SDL_QUIT:
                running = false;
                std::cout << "QUIT" << std::endl;
                break;
            case SDL_KEYDOWN:
                switch( e.key.keysym.sym ){
                case SDLK_ESCAPE:
                    running = false;
                    std::cout << "ESC" << std::endl;
                    break;
                case SDLK_d:
                    sim.incDiff();
                    std::cout << "next diff" << std::endl;
                    break;
                case SDLK_0:
                    sim.setDiff(0);
                    break;
                case SDLK_1:
                    sim.setDiff(1);
                    break;
                case SDLK_2:
                    sim.setDiff(2);
                    break;
                case SDLK_3:
                    sim.setDiff(3);
                    break;
                case SDLK_4:
                    sim.setDiff(4);
                    break;
                default:
                    std::cout << e.key.keysym.scancode << ":" << e.key.keysym.sym << ":" << e.key.type << std::endl;
                    break;
                }
                break;
            default:
                break;
            }
        }

        //get_from_UI(dens_prev, u_prev, v_prev);
        //sim.set_source(N, dens, sources, dt);
        sim.copy_values(N, u_prev, u, 1.);
        sim.copy_values(N, v_prev, v, 1.);
        sim.set_source(N, u_prev, force_u, 1.);
        sim.set_source(N, v_prev, force_v, 1.);

        sim.copy_values(N, dens_prev, dens, 1.);
        sim.set_source(N, dens, sources, 1.);

        for(int i=0; i<M_size; i++){
                dens[i] = 0.;
                u[i] = 0.;
                v[i] = 0.;
        }

        sim.vel_step(N, u, v, u_prev, v_prev, visc, dt);
        sim.dens_step(N, dens, dens_prev, u, v, diff, dt);
        sim.set_source(N, dens, sources, 1.);

        //Initialize renderer color
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear( renderer );

        SDL_Texture* texture_2 = sim.render(N, dens, texture);
        if( nullptr == texture_2 ) { throw SdlError("Could not create RGB Texture!"); }
        SDL_RenderCopy( renderer, texture, nullptr, nullptr );


        //Update screen
        SDL_RenderPresent( renderer );

        uint32_t t_n = SDL_GetTicks();
        uint32_t t_d = t_n - t_0;
        uint32_t t_r = 1000 / 30;
        uint32_t delay = (t_r > t_d) ? (t_r - t_d) : 0;
        holeTime += t_d;

        std::cout << "fps: " << 1000. / t_d << "\n"
                  << "ave: " << 1000. * iter / holeTime << "\n"
                  << "\n\n";
        SDL_Delay(delay);
    }
    //Free loaded image
    SDL_DestroyTexture( texture );
    texture = nullptr;
    //if(!saveScreenshotBMP("stable-100.bmp", window, renderer)){ std::cerr << "could not save pic." << std::endl; }
}

SDL_Surface* SdlApp::loadSurface( std::string path ){
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr ) { throw SdlImgError("Unable to load image " + path); }

    //Convert surface to screen format
    SDL_Surface* optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
    if( optimizedSurface == nullptr ) { throw SdlError("Unable to optimize image " + path); }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
    loadedSurface = nullptr;

    return optimizedSurface;
}

SDL_Texture* SdlApp::loadTexture( std::string path ){
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr ) { throw SdlImgError("Unable to load image " + path); }

    //The final texture
    //Create texture from surface pixels
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == nullptr ) { throw SdlError("Unable to create texture from " + path); }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    return newTexture;
}

bool SdlApp::saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(SDLWindow);
    if (infoSurface == NULL) {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    } else {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0) {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        } else {
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                pixels = NULL;
                return false;
            } else {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL) {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    return false;
                }
                SDL_SaveBMP(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }
    return true;
}
