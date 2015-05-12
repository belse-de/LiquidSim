#ifndef SDLTEXTURE_H
#define SDLTEXTURE_H

#include <string>

#include <SDL.h>

class SdlTexture
{
    public:
        /** Default constructor */
        SdlTexture( SDL_Renderer* renderer, std::string& path );
        /** Default destructor */
        virtual ~SdlTexture();

        void render( int x, int y, SDL_Rect* clip = nullptr );

        /** Access m_width
         * \return The current value of m_width
         */
        int getWidth() { return m_width; }
        /** Access m_height
         * \return The current value of m_height
         */
        int getHeight() { return m_height; }
        void setColor(uint8_t red, uint8_t green, uint8_t blue);
        void setAlpha(uint8_t alpha);
        void setBlendMode( SDL_BlendMode mode);

    protected:
    private:
        SDL_Renderer* m_renderer;
        std::string m_path;
        int m_width; //!< Member variable "m_width"
        int m_height; //!< Member variable "m_height"
        //The actual hardware texture
		SDL_Texture* m_texture;

};

#endif // SDLTEXTURE_H
