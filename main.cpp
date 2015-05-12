#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_net.h>

#include "main.hpp"

//SDL needs this structure because it uses a macro to exchange
int main(int argC, char** argS)
{
    std::cout << "Hello world!" << std::endl;
    for( int i = 0; i < argC; i++){
        std::cout << "Argument " << i << " : " << argS[i] << std::endl;
    }

    try{
        SdlApp app(480, 480);
        app.draw();
        app.loop();
    } catch(SdlError& e){
        std::cerr << "SDL: " << newline << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch(SdlImgError& e){
        std::cerr << "IMG: " << newline << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

