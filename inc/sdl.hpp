#ifndef SDL_APP_H
#define SDL_APP_H

#include <SDL.h>


class SdlApp
{
    public:
        SdlApp(int width = 640, int height = 480);
        virtual ~SdlApp();

        bool draw();
        void loop();
        SDL_Surface* loadSurface( std::string path );
        SDL_Texture* loadTexture( std::string path );
        bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);
    protected:
    private:
        //Screen dimension constants
        const int screenWidth;
        const int screenHeight;

        //The window we'll be rendering to
        SDL_Window* window;

        //The surface contained by the window
        SDL_Surface* screenSurface;

        //The window renderer
        SDL_Renderer* renderer;
};

#endif // SDL_APP_H
